#!/usr/bin/env python3
"""
AtCoder AHC048 - 本格版大量データ学習システム
500個の実データで高精度パラメータ最適化を学習する

Usage:
    python train_params_production.py
    → lgbm_params_production.json が生成される
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

# 本格版設定
SOLVER_BIN = "./solver_eval"
MODEL_JSON = "lgbm_params_production.json"
INPUT_DIR = "./input"
CACHE_DIR = "./cache_production"
MAX_WORKERS = min(8, mp.cpu_count())

# 本格版学習設定
PRODUCTION_SAMPLE_SIZE = 250         # 500個から250個を代表選択
USE_SMART_SAMPLING = True           # 代表性のあるデータ選択
USE_CACHE = True                    # 計算結果キャッシュ
ENABLE_ENSEMBLE = True              # アンサンブル学習
ENABLE_ADVANCED_FEATURES = True     # 高度特徴量

# パラメータ探索範囲（本格版）
LOOKAHEAD_RANGE = [1, 2, 3, 4, 5]      
BEAM_RANGE = [4, 6, 8, 12, 16, 20, 24]  
PALETTE_RANGE = [0, 1, 2, 3, 4]        

print(f"🚀 AtCoder AHC048 本格版大量データ学習システム")
print(f"   並列処理: {MAX_WORKERS}コア")
print(f"   サンプルサイズ: {PRODUCTION_SAMPLE_SIZE}")

# キャッシュディレクトリ作成
if USE_CACHE:
    os.makedirs(CACHE_DIR, exist_ok=True)

def extract_advanced_features(inp: str) -> np.ndarray:
    """高度な特徴量を抽出（22次元）"""
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
    
    # 基本特徴量（4次元）
    features = [K, H, T, D]
    
    # 所有色の高度統計量（6次元）
    own_arr = np.array(own_colors)
    features.extend([
        np.var(own_arr[:, 0]),   # R分散
        np.var(own_arr[:, 1]),   # G分散 
        np.var(own_arr[:, 2]),   # B分散
        np.ptp(own_arr[:, 0]),   # R範囲
        np.ptp(own_arr[:, 1]),   # G範囲
        np.ptp(own_arr[:, 2]),   # B範囲
    ])
    
    # ターゲット色の統計量（3次元）
    target_arr = np.array(target_colors)
    features.extend([
        np.var(target_arr[:, 0]),   # ターゲットR分散
        np.var(target_arr[:, 1]),   # ターゲットG分散
        np.var(target_arr[:, 2]),   # ターゲットB分散
    ])
    
    # 色空間カバー率（2次元）
    min_dists = []
    for tr, tg, tb in target_colors:
        min_dist = min(((tr-or_)**2 + (tg-og)**2 + (tb-ob)**2)**0.5 
                      for or_, og, ob in own_colors)
        min_dists.append(min_dist)
    features.append(np.mean(min_dists))  # 平均最小距離
    features.append(np.std(min_dists))   # 距離の標準偏差
    
    # コスト効率指標（2次元）
    features.append(T / H)               # ターン/ターゲット比
    features.append(D / T)               # コスト密度
    
    if ENABLE_ADVANCED_FEATURES:
        # 高度特徴量（5次元）
        features.append(K / H)           # 色/ターゲット比
        features.append(np.log(T + 1))   # ログターン数
        features.append(np.log(D + 1))   # ログコスト
        
        # 色分布の複雑性
        own_distances = []
        for i in range(len(own_colors)):
            for j in range(i + 1, len(own_colors)):
                dist = ((own_colors[i][0] - own_colors[j][0])**2 + 
                       (own_colors[i][1] - own_colors[j][1])**2 + 
                       (own_colors[i][2] - own_colors[j][2])**2)**0.5
                own_distances.append(dist)
        
        features.append(np.mean(own_distances) if own_distances else 0)  # 所有色間距離
        features.append(np.std(own_distances) if own_distances else 0)   # 距離分散
    
    return np.array(features, dtype=np.float32)

def smart_sample_selection(files, target_size):
    """代表性のあるデータを選択"""
    print(f"🧠 スマートサンプリング: {len(files)}個 → {target_size}個")
    
    if len(files) <= target_size:
        return files
    
    # まず100個で特徴量分析
    sample_files = random.sample(files, min(100, len(files)))
    features_list = []
    valid_files = []
    
    for file_path in tqdm(sample_files, desc="特徴量分析"):
        try:
            with open(file_path, 'r') as f:
                content = f.read().strip()
            feature = extract_advanced_features(content)
            features_list.append(feature)
            valid_files.append(file_path)
        except:
            continue
    
    if len(features_list) < 20:
        print("⚠️ 分析データ不足、ランダム選択に切り替え")
        return random.sample(files, target_size)
    
    # K-meansクラスタリングで多様性確保
    n_clusters = min(target_size // 5, len(features_list) // 3)
    kmeans = KMeans(n_clusters=n_clusters, random_state=42, n_init=10)
    cluster_labels = kmeans.fit_predict(features_list)
    
    selected_files = set()
    
    # 各クラスタから代表選択
    for cluster_id in range(n_clusters):
        cluster_files = [valid_files[i] for i, label in enumerate(cluster_labels) if label == cluster_id]
        cluster_size = min(len(cluster_files), target_size // n_clusters + 1)
        selected_files.update(random.sample(cluster_files, cluster_size))
    
    # 残りをランダム選択で埋める
    remaining_files = [f for f in files if f not in selected_files]
    need_more = target_size - len(selected_files)
    if need_more > 0:
        selected_files.update(random.sample(remaining_files, min(need_more, len(remaining_files))))
    
    result = list(selected_files)[:target_size]
    print(f"✅ 多様性を考慮した{len(result)}個を選択")
    return result

def evaluate_with_cache(inp: str, L: int, B: int, P: int) -> float:
    """キャッシュ付き評価関数"""
    cache_key = f"{hash(inp) % 1000000}_{L}_{B}_{P}"
    cache_file = os.path.join(CACHE_DIR, f"{cache_key}.txt") if USE_CACHE else None
    
    # キャッシュ確認
    if cache_file and os.path.exists(cache_file):
        try:
            with open(cache_file, 'r') as f:
                return float(f.read().strip())
        except:
            pass
    
    # 実際の評価
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as f:
            f.write(inp)
            input_file = f.name
        
        cmd = [SOLVER_BIN, str(L), str(B), str(P)]
        
        with open(input_file, 'r') as f:
            result = subprocess.run(
                cmd, stdin=f, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=25, text=True
            )
        
        if result.returncode == 0 and result.stderr:
            try:
                score = float(result.stderr.strip().split()[-1])
                
                # キャッシュ保存
                if cache_file:
                    try:
                        with open(cache_file, 'w') as f:
                            f.write(str(score))
                    except:
                        pass
                
                return score
            except:
                pass
        
        return 1e9
        
    except subprocess.TimeoutExpired:
        return 1e9
    except Exception as e:
        return 1e9
    finally:
        if 'input_file' in locals():
            try:
                os.unlink(input_file)
            except:
                pass

def find_best_params_production(inp: str) -> tuple[int, int, int, float]:
    """本格版パラメータ探索（より細かく）"""
    best_score = 1e9
    best_params = (2, 8, 0)
    evaluations = 0
    max_evaluations = 20  # 本格版では精密に
    
    # フェーズ1: 基本候補
    phase1_candidates = [
        (1, 4, 0), (1, 8, 1), (2, 8, 0), (2, 12, 2), 
        (3, 16, 1), (4, 20, 3), (5, 24, 4),
        (2, 6, 1), (3, 12, 0), (4, 16, 2)
    ]
    
    promising = []
    
    for L, B, P in phase1_candidates:
        if evaluations >= max_evaluations // 2:
            break
        score = evaluate_with_cache(inp, L, B, P)
        evaluations += 1
        
        if score < best_score:
            best_score = score
            best_params = (L, B, P)
        
        promising.append((L, B, P, score))
    
    # フェーズ2: 上位候補周辺を精密探索
    promising.sort(key=lambda x: x[3])
    top_3 = promising[:3]
    
    for base_L, base_B, base_P, _ in top_3:
        for dL in [-1, 0, 1]:
            for dB in [-4, 0, 4]:
                if evaluations >= max_evaluations:
                    break
                
                L = max(1, min(5, base_L + dL))
                B = max(4, min(24, base_B + dB))
                P = base_P  # パレットは固定
                
                if B not in BEAM_RANGE:
                    continue
                
                # 既評価をスキップ
                if (L, B, P) in [(c[0], c[1], c[2]) for c in promising]:
                    continue
                
                score = evaluate_with_cache(inp, L, B, P)
                evaluations += 1
                
                if score < best_score:
                    best_score = score
                    best_params = (L, B, P)
    
    return (*best_params, best_score)

def process_single_file_production(file_path):
    """本格版ファイル処理"""
    try:
        with open(file_path, 'r') as f:
            inp = f.read().strip()
        
        features = extract_advanced_features(inp)
        L, B, P, score = find_best_params_production(inp)
        return features, (L, B, P), score, os.path.basename(file_path)
    except Exception as e:
        print(f"⚠️ ファイル{file_path}でエラー: {e}")
        return None

def create_production_dataset():
    """本格版データセット作成"""
    print("🏭 本格版データセット作成中...")
    
    # 実データの数を確認
    real_data_files = sorted(glob.glob(os.path.join(INPUT_DIR, "*.txt")))
    N_REAL_DATA = len(real_data_files)
    
    print(f"   発見されたデータ: {N_REAL_DATA}個")
    
    if N_REAL_DATA == 0:
        print(f"❌ エラー: {INPUT_DIR} に.txtファイルが見つかりません")
        return None, None, None
    
    # スマートサンプリング
    if USE_SMART_SAMPLING and N_REAL_DATA > PRODUCTION_SAMPLE_SIZE:
        selected_files = smart_sample_selection(real_data_files, PRODUCTION_SAMPLE_SIZE)
    else:
        selected_files = real_data_files[:PRODUCTION_SAMPLE_SIZE]
    
    print(f"✅ {len(selected_files)}個のデータで本格学習")
    
    # 並列処理
    with mp.Pool(MAX_WORKERS) as pool:
        results = list(tqdm(
            pool.imap(process_single_file_production, selected_files), 
            total=len(selected_files),
            desc="本格処理"
        ))
    
    valid_results = [r for r in results if r is not None]
    print(f"✅ 有効データ: {len(valid_results)}/{len(selected_files)}")
    
    if len(valid_results) < 20:
        print("⚠️ 有効データが少なすぎます。")
        return None, None, None
    
    X = np.vstack([r[0] for r in valid_results])
    params = np.array([r[1] for r in valid_results])
    scores = np.array([r[2] for r in valid_results])
    filenames = [r[3] for r in valid_results]
    
    print(f"📈 スコア統計: 平均={np.mean(scores):.1f}, 最小={np.min(scores):.1f}, 最大={np.max(scores):.1f}")
    print(f"📈 パラメータ分布:")
    print(f"   L: {np.mean(params[:, 0]):.2f} ± {np.std(params[:, 0]):.2f} (範囲: {np.min(params[:, 0])}-{np.max(params[:, 0])})")
    print(f"   B: {np.mean(params[:, 1]):.2f} ± {np.std(params[:, 1]):.2f} (範囲: {np.min(params[:, 1])}-{np.max(params[:, 1])})")
    print(f"   P: {np.mean(params[:, 2]):.2f} ± {np.std(params[:, 2]):.2f} (範囲: {np.min(params[:, 2])}-{np.max(params[:, 2])})")
    
    return X, params, scores

def train_production_models(X, Y):
    """本格版アンサンブルモデル学習"""
    print("🎓 本格版アンサンブルモデル訓練中...")
    
    # 訓練・検証分割
    X_train, X_val, Y_train, Y_val = train_test_split(X, Y, test_size=0.2, random_state=42)
    
    # LightGBM高精度パラメータ
    lgb_params = {
        'objective': 'regression',
        'metric': 'rmse',
        'boosting_type': 'gbdt',
        'num_leaves': 127,
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
        print(f"  📚 {name} アンサンブル訓練中...")
        
        if ENABLE_ENSEMBLE:
            # アンサンブル: 3つのモデル
            ensemble = []
            for seed in [42, 123, 456]:
                lgb_params_copy = lgb_params.copy()
                lgb_params_copy['random_state'] = seed
                model = lgb.LGBMRegressor(
                    **lgb_params_copy,
                    n_estimators=500  # より多くの木
                )
                model.fit(X_train, Y_train[:, i])
                ensemble.append(model)
            
            # アンサンブル予測評価
            pred_ensemble = np.mean([m.predict(X_val) for m in ensemble], axis=0)
            mae = np.mean(np.abs(pred_ensemble - Y_val[:, i]))
            rmse = np.sqrt(np.mean((pred_ensemble - Y_val[:, i])**2))
            
            models.append(ensemble)
            print(f"    ✅ MAE: {mae:.3f}, RMSE: {rmse:.3f}")
        else:
            model = lgb.LGBMRegressor(**lgb_params, n_estimators=500)
            model.fit(X_train, Y_train[:, i])
            
            pred = model.predict(X_val)
            mae = np.mean(np.abs(pred - Y_val[:, i]))
            models.append([model])
            print(f"    ✅ MAE: {mae:.3f}")
    
    return models

def save_production_models(models):
    """本格版モデル保存"""
    print("💾 本格版アンサンブルモデル保存中...")
    
    model_data = []
    for model_ensemble in models:
        if isinstance(model_ensemble, list) and len(model_ensemble) > 1:
            # アンサンブルの場合
            ensemble_data = []
            for model in model_ensemble:
                model_json = model.booster_.dump_model()
                ensemble_data.append(model_json)
            model_data.append(ensemble_data)
        else:
            # 単一モデルの場合
            model = model_ensemble[0] if isinstance(model_ensemble, list) else model_ensemble
            model_json = model.booster_.dump_model()
            model_data.append([model_json])
    
    with open(MODEL_JSON, 'w') as f:
        json.dump(model_data, f, indent=2)
    
    print(f"✅ 本格版アンサンブルモデル保存完了: {MODEL_JSON}")

def main():
    print("🚀 AtCoder AHC048 本格版大量データ学習システム")
    print("=" * 70)
    
    if not os.path.exists(INPUT_DIR):
        print(f"❌ エラー: {INPUT_DIR} ディレクトリが見つかりません")
        return
    
    if not os.path.exists(SOLVER_BIN):
        print(f"❌ エラー: {SOLVER_BIN} が見つかりません")
        return
    
    start_time = time.time()
    
    # データセット作成
    X, params, scores = create_production_dataset()
    if X is None:
        return
    
    print(f"📊 特徴量次元: {X.shape[1]}")
    print(f"📊 データ数: {X.shape[0]}")
    print(f"📊 アンサンブル: {'有効' if ENABLE_ENSEMBLE else '無効'}")
    
    # モデル学習
    models = train_production_models(X, params)
    save_production_models(models)
    
    elapsed = time.time() - start_time
    print(f"⏱️ 本格版学習完了: {elapsed:.1f}秒")
    print(f"🎉 高精度{MODEL_JSON}が生成されました！")
    
    print("\n🏆 本格版完成！次は:")
    print("   mv lgbm_params_production.json lgbm_params.json")
    print("   python gen_header_advanced.py")
    print("   g++ -O2 -std=c++20 -o solver_final solver_ml.cpp")

if __name__ == "__main__":
    main() 