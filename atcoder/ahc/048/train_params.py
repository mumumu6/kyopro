#!/usr/bin/env python3
"""
AtCoder Heuristic Contest 048 - 大量データ対応自動パラメータ最適化学習システム
500個の実データを効率的にサンプリングして最適な(L=先読み, B=ビーム幅, P=パレット)を学習する

Usage:
    python train_params.py
    → lgbm_params.json が生成される
"""

import json, subprocess, random, pathlib, tempfile, shutil, multiprocessing as mp
import numpy as np
import lightgbm as lgb
from tqdm import tqdm
import math
import time
import os
import sys
import glob
from sklearn.model_selection import train_test_split
from sklearn.cluster import KMeans
import pickle

# 設定
SOLVER_BIN = "./solver_eval"           # 評価専用ソルバ (後で作成)
MODEL_JSON = "lgbm_params.json"
INPUT_DIR = "./input"                  # 実データディレクトリ
CACHE_DIR = "./cache"                  # 計算結果キャッシュ用
MAX_WORKERS = min(12, mp.cpu_count())  # 並列数

# 大量データ対応戦略設定
USE_SMART_SAMPLING = True              # 代表的なデータのみ学習するか
SMART_SAMPLE_SIZE = 200               # 代表データ数（500個から200個を選択）
USE_CACHE = True                      # 計算結果キャッシュを使うか
ENABLE_ADVANCED_FEATURES = True       # 高度な特徴量を使うか
USE_ENSEMBLE = True                   # アンサンブル学習するか

# パラメータ探索範囲（大量データなのでより細かく）
LOOKAHEAD_RANGE = [1, 2, 3, 4, 5]      # 先読み深さ
BEAM_RANGE = [4, 6, 8, 12, 16, 20, 24]  # ビーム幅（より細かく）
PALETTE_RANGE = [0, 1, 2, 3, 4]        # パレットレイアウトID

# 実データの数を確認
real_data_files = sorted(glob.glob(os.path.join(INPUT_DIR, "*.txt")))
N_REAL_DATA = len(real_data_files)

print(f"🎯 大量データ対応学習システム起動！")
print(f"   発見されたデータ: {N_REAL_DATA}個")
print(f"   スマートサンプリング: {'ON' if USE_SMART_SAMPLING else 'OFF'}")
print(f"   並列処理: {MAX_WORKERS}コア")

# キャッシュディレクトリ作成
if USE_CACHE:
    os.makedirs(CACHE_DIR, exist_ok=True)

# ------------------ スマートサンプリング ------------------
def analyze_data_diversity(files):
    """データの多様性を分析して代表的なケースを選出"""
    print("🔍 データ多様性分析中...")
    
    features_for_clustering = []
    file_features = {}
    
    for file_path in tqdm(files[:100], desc="特徴量抽出"):  # 全部は重いので100個で分析
        try:
            with open(file_path, 'r') as f:
                content = f.read().strip()
            feature = extract_feature(content)
            features_for_clustering.append(feature)
            file_features[file_path] = feature
        except:
            continue
    
    if len(features_for_clustering) < 10:
        print("⚠️ 分析用データが不足。全データを使用します。")
        return files
    
    # K-meansクラスタリングで代表ケースを選出
    n_clusters = min(SMART_SAMPLE_SIZE // 5, len(features_for_clustering) // 2)
    kmeans = KMeans(n_clusters=n_clusters, random_state=42, n_init=10)
    cluster_labels = kmeans.fit_predict(features_for_clustering)
    
    # 各クラスタから代表的なケースを選出
    selected_files = []
    for cluster_id in range(n_clusters):
        cluster_files = [files[i] for i, label in enumerate(cluster_labels) if label == cluster_id]
        # クラスタ中心に最も近いケースを選ぶ
        cluster_center = kmeans.cluster_centers_[cluster_id]
        best_file = cluster_files[0]
        best_dist = float('inf')
        
        for file_path in cluster_files:
            if file_path in file_features:
                dist = np.linalg.norm(file_features[file_path] - cluster_center)
                if dist < best_dist:
                    best_dist = dist
                    best_file = file_path
        
        selected_files.append(best_file)
        
        # クラスタサイズに応じて追加選択
        additional = min(len(cluster_files) - 1, SMART_SAMPLE_SIZE // n_clusters - 1)
        for i in range(additional):
            if i + 1 < len(cluster_files):
                selected_files.append(cluster_files[i + 1])
    
    print(f"✅ {len(selected_files)}個の代表ケースを選出")
    return selected_files[:SMART_SAMPLE_SIZE]

# ------------------ 高度な特徴量抽出 ------------------
def extract_advanced_features(inp: str) -> np.ndarray:
    """高度な特徴量を抽出（大量データ対応）"""
    lines = inp.strip().split('\n')
    N, K, H, T, D = map(int, lines[0].split())
    
    # 所有色の統計量
    own_colors = []
    for i in range(1, K + 1):
        r, g, b = map(float, lines[i].split())
        own_colors.append((r, g, b))
    
    # ターゲット色の統計量  
    target_colors = []
    for i in range(K + 1, K + 1 + H):
        r, g, b = map(float, lines[i].split())
        target_colors.append((r, g, b))
    
    # 基本特徴量
    features = [K, H, T, D]
    
    # 所有色の分散・範囲
    own_arr = np.array(own_colors)
    features.extend([
        np.var(own_arr[:, 0]),   # R分散
        np.var(own_arr[:, 1]),   # G分散 
        np.var(own_arr[:, 2]),   # B分散
        np.ptp(own_arr[:, 0]),   # R範囲
        np.ptp(own_arr[:, 1]),   # G範囲
        np.ptp(own_arr[:, 2]),   # B範囲
    ])
    
    # ターゲット色の統計量
    target_arr = np.array(target_colors)
    features.extend([
        np.var(target_arr[:, 0]),   # ターゲットR分散
        np.var(target_arr[:, 1]),   # ターゲットG分散
        np.var(target_arr[:, 2]),   # ターゲットB分散
    ])
    
    # 色空間カバー率
    min_dists = []
    for tr, tg, tb in target_colors:
        min_dist = min(((tr-or_)**2 + (tg-og)**2 + (tb-ob)**2)**0.5 
                      for or_, og, ob in own_colors)
        min_dists.append(min_dist)
    features.append(np.mean(min_dists))  # 平均最小距離
    features.append(np.std(min_dists))   # 距離の標準偏差
    
    # コスト効率指標
    features.append(T / H)               # ターン/ターゲット比
    features.append(D / T)               # コスト密度
    
    if ENABLE_ADVANCED_FEATURES:
        # 高度な特徴量
        features.append(K / H)           # 色/ターゲット比
        features.append(np.log(T + 1))   # ログターン数
        features.append(np.log(D + 1))   # ログコスト
        
        # 色分布のエントロピー
        own_distances = []
        for i in range(len(own_colors)):
            for j in range(i + 1, len(own_colors)):
                dist = ((own_colors[i][0] - own_colors[j][0])**2 + 
                       (own_colors[i][1] - own_colors[j][1])**2 + 
                       (own_colors[i][2] - own_colors[j][2])**2)**0.5
                own_distances.append(dist)
        
        features.append(np.mean(own_distances) if own_distances else 0)  # 所有色間距離
        features.append(np.std(own_distances) if own_distances else 0)   # 距離分散
        
        # 難易度指標
        difficulty = np.mean(min_dists) * D / T + K / (H + 1)
        features.append(difficulty)
    
    return np.array(features, dtype=np.float32)

# 既存のextract_feature関数を置き換え
extract_feature = extract_advanced_features if ENABLE_ADVANCED_FEATURES else extract_feature

# ------------------ 入力生成 ------------------
def gen_case(seed: int) -> str:
    """AHC048の問題形式に合わせて入力を生成"""
    random.seed(seed)
    np.random.seed(seed)
    
    # 問題パラメータ生成 (実際のコンテスト分布に近似)
    N = 20
    K = random.randint(4, 20)
    H = random.randint(800, 1000)
    T = round(5000 * (2**random.uniform(0, 3.5)))  # 5000-40000程度
    D = round(10 ** random.uniform(1.5, 3.5))      # 32-3162程度
    
    # 所有色生成 (RGB各成分0-1の実数)
    own_colors = []
    for _ in range(K):
        # 少し偏りを持たせて現実的な色分布にする
        r = np.clip(np.random.beta(2, 2), 0, 1)
        g = np.clip(np.random.beta(2, 2), 0, 1) 
        b = np.clip(np.random.beta(2, 2), 0, 1)
        own_colors.append((r, g, b))
    
    # ターゲット色生成
    target_colors = []
    for _ in range(H):
        # ターゲットは所有色から作れる範囲で生成（学習効率向上）
        if random.random() < 0.7 and len(own_colors) >= 2:
            # 70%の確率で所有色の線形結合から生成
            c1, c2 = random.sample(own_colors, 2)
            alpha = random.random()
            r = alpha * c1[0] + (1-alpha) * c2[0]
            g = alpha * c1[1] + (1-alpha) * c2[1]
            b = alpha * c1[2] + (1-alpha) * c2[2]
        else:
            # 30%は完全ランダム（難しいケース）
            r = random.random()
            g = random.random()
            b = random.random()
        target_colors.append((r, g, b))
    
    # 入力文字列生成
    lines = [f"{N} {K} {H} {T} {D}"]
    for r, g, b in own_colors:
        lines.append(f"{r:.6f} {g:.6f} {b:.6f}")
    for r, g, b in target_colors:
        lines.append(f"{r:.6f} {g:.6f} {b:.6f}")
    
    return "\n".join(lines)

# ------------------ 評価関数 ------------------
def evaluate_with_timeout(inp: str, L: int, B: int, P: int, timeout: int = 30) -> float:
    """指定パラメータでソルバを実行してスコアを取得（タイムアウト付き）"""
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as f:
            f.write(inp)
            input_file = f.name
        
        # ソルバ実行 (L, B, P をコマンドライン引数で渡す)
        cmd = [SOLVER_BIN, str(L), str(B), str(P)]
        
        with open(input_file, 'r') as f:
            result = subprocess.run(
                cmd, stdin=f, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=timeout, text=True
            )
        
        # スコアをstderrから取得（実装に応じて調整）
        if result.returncode == 0 and result.stderr:
            try:
                score = float(result.stderr.strip().split()[-1])
                return score
            except:
                pass
        
        # fallback: 巨大なペナルティ
        return 1e9
        
    except subprocess.TimeoutExpired:
        return 1e9
    except Exception as e:
        print(f"⚠️ 評価エラー: {e}")
        return 1e9
    finally:
        if 'input_file' in locals():
            try:
                os.unlink(input_file)
            except:
                pass

# ------------------ インテリジェント探索 ------------------
def intelligent_param_search(inp: str, max_evaluations: int = 35) -> tuple[int, int, int, float]:
    """ベイジアン最適化風のインテリジェント探索"""
    
    # キャッシュチェック
    cache_key = hash(inp) % 1000000
    cache_file = os.path.join(CACHE_DIR, f"cache_{cache_key}.pkl") if USE_CACHE else None
    
    if cache_file and os.path.exists(cache_file):
        try:
            with open(cache_file, 'rb') as f:
                return pickle.load(f)
        except:
            pass
    
    best_score = 1e9
    best_params = (2, 8, 0)
    evaluations = 0
    
    # フェーズ1: 粗い探索で有望領域を特定
    promising_candidates = []
    phase1_candidates = [
        (1, 4, 0), (1, 8, 1), (2, 8, 0), (2, 12, 2), 
        (3, 16, 1), (4, 20, 3), (5, 24, 4)
    ]
    
    for L, B, P in phase1_candidates:
        if B not in BEAM_RANGE or evaluations >= max_evaluations // 2:
            continue
        score = evaluate_with_timeout(inp, L, B, P, timeout=20)
        evaluations += 1
        
        if score < best_score:
            best_score = score
            best_params = (L, B, P)
        
        # 上位候補として記録
        promising_candidates.append((L, B, P, score))
    
    # 上位候補を中心に精密探索
    promising_candidates.sort(key=lambda x: x[3])
    top_candidates = promising_candidates[:3]
    
    # フェーズ2: 上位候補周辺の精密探索
    for base_L, base_B, base_P, _ in top_candidates:
        for dL in [-1, 0, 1]:
            for dB in [-4, 0, 4]:
                for dP in [-1, 0, 1]:
                    if evaluations >= max_evaluations:
                        break
                    
                    L = max(1, min(5, base_L + dL))
                    B = max(4, min(24, base_B + dB))
                    P = max(0, min(4, base_P + dP))
                    
                    if B not in BEAM_RANGE:
                        continue
                    
                    # 既に評価済みをスキップ
                    if (L, B, P) in [(c[0], c[1], c[2]) for c in promising_candidates]:
                        continue
                    
                    score = evaluate_with_timeout(inp, L, B, P, timeout=25)
                    evaluations += 1
                    
                    if score < best_score:
                        best_score = score
                        best_params = (L, B, P)
    
    result = (*best_params, best_score)
    
    # 結果をキャッシュ
    if cache_file:
        try:
            with open(cache_file, 'wb') as f:
                pickle.dump(result, f)
        except:
            pass
    
    return result

# find_best_paramsを置き換え
find_best_params = intelligent_param_search

# ------------------ データセット作成 ------------------
def create_smart_dataset():
    """スマートサンプリング＆効率的データセット作成"""
    print("📊 スマートデータセット作成中...")
    
    # 実データが存在するかチェック
    if N_REAL_DATA == 0:
        print(f"❌ エラー: {INPUT_DIR} に.txtファイルが見つかりません")
        return None, None, None
    
    # スマートサンプリング
    if USE_SMART_SAMPLING and N_REAL_DATA > SMART_SAMPLE_SIZE:
        selected_files = analyze_data_diversity(real_data_files)
        print(f"✅ {len(selected_files)}個の代表データで学習")
    else:
        selected_files = real_data_files
        print(f"✅ 全{len(selected_files)}個のデータで学習")
    
    def worker(file_path):
        """1つのファイルに対してデータ作成"""
        try:
            with open(file_path, 'r') as f:
                inp = f.read().strip()
            
            features = extract_feature(inp)
            L, B, P, score = find_best_params(inp)
            return features, (L, B, P), score, os.path.basename(file_path)
        except Exception as e:
            print(f"⚠️ ファイル{file_path}でエラー: {e}")
            return None
    
    # 並列処理で高速化
    with mp.Pool(MAX_WORKERS) as pool:
        results = list(tqdm(
            pool.imap(worker, selected_files), 
            total=len(selected_files),
            desc="データ処理"
        ))
    
    # None除去
    valid_results = [r for r in results if r is not None]
    print(f"✅ 有効データ: {len(valid_results)}/{len(selected_files)}")
    
    if len(valid_results) < len(selected_files) * 0.7:
        print("⚠️ 有効データが少なすぎます。solver_evalの実装を確認してください。")
        return None, None, None
    
    X = np.vstack([r[0] for r in valid_results])
    params = np.array([r[1] for r in valid_results])
    scores = np.array([r[2] for r in valid_results])
    filenames = [r[3] for r in valid_results]
    
    print(f"📈 スコア統計: 平均={np.mean(scores):.1f}, 最小={np.min(scores):.1f}, 最大={np.max(scores):.1f}")
    
    # デバッグ情報
    print(f"📊 パラメータ分布:")
    print(f"   L (先読み): 平均={np.mean(params[:, 0]):.2f}, 範囲=[{np.min(params[:, 0])}-{np.max(params[:, 0])}]")
    print(f"   B (ビーム): 平均={np.mean(params[:, 1]):.2f}, 範囲=[{np.min(params[:, 1])}-{np.max(params[:, 1])}]")
    print(f"   P (パレット): 平均={np.mean(params[:, 2]):.2f}, 範囲=[{np.min(params[:, 2])}-{np.max(params[:, 2])}]")
    
    return X, params, scores

# create_datasetを置き換え
create_dataset = create_smart_dataset

# ------------------ モデル学習 ------------------
def train_ensemble_models(X, Y):
    """アンサンブル学習でより高精度なモデルを作成"""
    print("🤖 アンサンブル機械学習モデル訓練中...")
    
    # 訓練・検証分割
    X_train, X_val, Y_train, Y_val = train_test_split(X, Y, test_size=0.2, random_state=42)
    
    # LightGBMパラメータ（高精度設定）
    lgb_params = {
        'objective': 'regression',
        'metric': 'rmse',
        'boosting_type': 'gbdt',
        'num_leaves': 63,
        'learning_rate': 0.05,
        'feature_fraction': 0.8,
        'bagging_fraction': 0.8,
        'bagging_freq': 5,
        'verbose': -1,
        'random_state': 42,
        'reg_alpha': 0.1,
        'reg_lambda': 0.1
    }
    
    models = []
    param_names = ['L (lookahead)', 'B (beam)', 'P (palette)']
    
    for i, name in enumerate(param_names):
        print(f"  📚 {name} モデル訓練中...")
        
        if USE_ENSEMBLE:
            # アンサンブル: 3つの異なる設定でモデル作成
            ensemble = []
            for seed in [42, 123, 456]:
                model = lgb.LGBMRegressor(
                    **lgb_params,
                    n_estimators=300,
                    random_state=seed
                )
                model.fit(X_train, Y_train[:, i])
                ensemble.append(model)
            
            # アンサンブル予測をテスト
            pred_ensemble = np.mean([m.predict(X_val) for m in ensemble], axis=0)
            mae = np.mean(np.abs(pred_ensemble - Y_val[:, i]))
            
            models.append(ensemble)
        else:
            model = lgb.LGBMRegressor(
                **lgb_params,
                n_estimators=300
            )
            model.fit(X_train, Y_train[:, i])
            
            pred = model.predict(X_val)
            mae = np.mean(np.abs(pred - Y_val[:, i]))
            
            models.append([model])
        
        print(f"    ✅ MAE: {mae:.3f}")
    
    return models

# train_modelsを置き換え
train_models = train_ensemble_models if USE_ENSEMBLE else train_models

def save_ensemble_models(models):
    """アンサンブルモデルをJSONで保存"""
    print("💾 アンサンブルモデル保存中...")
    
    model_data = []
    for model_ensemble in models:
        if isinstance(model_ensemble, list):
            # アンサンブルの場合は平均用のモデル情報を保存
            ensemble_data = []
            for model in model_ensemble:
                model_json = model.booster_.dump_model()
                ensemble_data.append(model_json)
            model_data.append(ensemble_data)
        else:
            # 単一モデルの場合
            model_json = model_ensemble.booster_.dump_model()
            model_data.append([model_json])
    
    with open(MODEL_JSON, 'w') as f:
        json.dump(model_data, f, indent=2)
    
    print(f"✅ アンサンブルモデル保存完了: {MODEL_JSON}")

# save_modelsを置き換え
save_models = save_ensemble_models if USE_ENSEMBLE else save_models

# ------------------ メイン処理 ------------------
def main():
    print("🚀 AtCoder AHC048 大量データ対応インテリジェント最適化学習システム")
    print("=" * 80)
    
    # 実データディレクトリの確認
    if not os.path.exists(INPUT_DIR):
        print(f"❌ エラー: {INPUT_DIR} ディレクトリが見つかりません")
        return
    
    if N_REAL_DATA == 0:
        print(f"❌ エラー: {INPUT_DIR} に.txtファイルが見つかりません")
        return
    
    # solver_evalの存在確認
    if not os.path.exists(SOLVER_BIN):
        print(f"❌ エラー: {SOLVER_BIN} が見つかりません")
        print("先にsolver_eval.cppをコンパイルしてください:")
        print(f"  g++ -O2 -std=c++20 -o {SOLVER_BIN} solver_eval.cpp")
        return
    
    start_time = time.time()
    
    # データセット作成
    X, params, scores = create_dataset()
    if X is None:
        return
    
    print(f"📊 特徴量次元: {X.shape[1]}")
    print(f"📊 データ数: {X.shape[0]}")
    print(f"📊 実データベース: {N_REAL_DATA}ファイル（スマートサンプリング適用）")
    
    # モデル学習
    models = train_models(X, params)
    
    # モデル保存
    save_models(models)
    
    # 学習時間
    elapsed = time.time() - start_time
    print(f"⏱️ 学習完了: {elapsed:.1f}秒")
    print(f"🎉 高精度{MODEL_JSON}が生成されました！")
    print(f"💎 500個のデータから学習した最強モデルよ！")
    print("\n次のステップ:")
    print("1. python gen_header.py でC++ヘッダ生成")
    print("2. solver.cppをコンパイルして本番実行")

if __name__ == "__main__":
    main() 