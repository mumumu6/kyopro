#!/usr/bin/env python3
"""
AtCoder AHC048 - 軽量版学習システム（動作確認用）
30個程度のデータで動作テストを行う

Usage:
    python train_params_lite.py
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

# 軽量設定
SOLVER_BIN = "./solver_eval"
MODEL_JSON = "lgbm_params.json"
INPUT_DIR = "./input"
MAX_WORKERS = min(4, mp.cpu_count())

# 軽量版設定
LITE_SAMPLE_SIZE = 30                  # 30個のみでテスト
QUICK_EVALUATION = True                # 高速評価モード

# パラメータ探索範囲（軽量版）
LOOKAHEAD_RANGE = [1, 2, 3, 4, 5]
BEAM_RANGE = [4, 8, 12, 16, 20]
PALETTE_RANGE = [0, 1, 2, 3, 4]

# 実データの数を確認
real_data_files = sorted(glob.glob(os.path.join(INPUT_DIR, "*.txt")))
N_REAL_DATA = len(real_data_files)

print(f"🧪 軽量版学習システム（動作確認用）")
print(f"   発見されたデータ: {N_REAL_DATA}個")
print(f"   使用データ: {min(LITE_SAMPLE_SIZE, N_REAL_DATA)}個")

def extract_feature(inp: str) -> np.ndarray:
    """基本的な特徴量を抽出"""
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
    
    # 基本特徴量のみ（17次元）
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
    
    return np.array(features, dtype=np.float32)

def evaluate_with_timeout(inp: str, L: int, B: int, P: int, timeout: int = 15) -> float:
    """軽量版評価関数"""
    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as f:
            f.write(inp)
            input_file = f.name
        
        cmd = [SOLVER_BIN, str(L), str(B), str(P)]
        
        with open(input_file, 'r') as f:
            result = subprocess.run(
                cmd, stdin=f, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=timeout, text=True
            )
        
        if result.returncode == 0 and result.stderr:
            try:
                score = float(result.stderr.strip().split()[-1])
                return score
            except:
                pass
        
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

def find_best_params_lite(inp: str) -> tuple[int, int, int, float]:
    """軽量版パラメータ探索"""
    best_score = 1e9
    best_params = (2, 8, 0)
    
    # 粗い探索のみ（高速化）
    candidates = [
        (1, 4, 0), (2, 8, 0), (2, 12, 1), 
        (3, 16, 2), (4, 20, 3), (5, 16, 4)
    ]
    
    for L, B, P in candidates:
        score = evaluate_with_timeout(inp, L, B, P, timeout=10)
        if score < best_score:
            best_score = score
            best_params = (L, B, P)
    
    return (*best_params, best_score)

def create_lite_dataset():
    """軽量版データセット作成"""
    print("📊 軽量版データセット作成中...")
    
    if N_REAL_DATA == 0:
        print(f"❌ エラー: {INPUT_DIR} に.txtファイルが見つかりません")
        return None, None, None
    
    # 最初のn個のみ使用
    selected_files = real_data_files[:min(LITE_SAMPLE_SIZE, N_REAL_DATA)]
    print(f"✅ {len(selected_files)}個のデータで軽量学習")
    
    def worker(file_path):
        try:
            with open(file_path, 'r') as f:
                inp = f.read().strip()
            
            features = extract_feature(inp)
            L, B, P, score = find_best_params_lite(inp)
            return features, (L, B, P), score
        except Exception as e:
            print(f"⚠️ ファイル{file_path}でエラー: {e}")
            return None
    
    with mp.Pool(MAX_WORKERS) as pool:
        results = list(tqdm(
            pool.imap(worker, selected_files), 
            total=len(selected_files),
            desc="軽量処理"
        ))
    
    valid_results = [r for r in results if r is not None]
    print(f"✅ 有効データ: {len(valid_results)}/{len(selected_files)}")
    
    if len(valid_results) < 5:
        print("⚠️ 有効データが少なすぎます。")
        return None, None, None
    
    X = np.vstack([r[0] for r in valid_results])
    params = np.array([r[1] for r in valid_results])
    scores = np.array([r[2] for r in valid_results])
    
    print(f"📈 スコア統計: 平均={np.mean(scores):.1f}, 最小={np.min(scores):.1f}")
    
    return X, params, scores

def train_lite_models(X, Y):
    """軽量版モデル学習"""
    print("🤖 軽量版モデル訓練中...")
    
    lgb_params = {
        'objective': 'regression',
        'metric': 'rmse',
        'boosting_type': 'gbdt',
        'num_leaves': 15,  # 軽量化
        'learning_rate': 0.1,
        'verbose': -1,
        'random_state': 42
    }
    
    models = []
    param_names = ['L (lookahead)', 'B (beam)', 'P (palette)']
    
    for i, name in enumerate(param_names):
        print(f"  📚 {name} モデル訓練中...")
        
        model = lgb.LGBMRegressor(
            **lgb_params,
            n_estimators=50  # 軽量化
        )
        
        model.fit(X, Y[:, i])
        models.append(model)
        
        pred = model.predict(X)
        mae = np.mean(np.abs(pred - Y[:, i]))
        print(f"    ✅ MAE: {mae:.3f}")
    
    return models

def save_lite_models(models):
    """軽量版モデル保存"""
    print("💾 軽量版モデル保存中...")
    
    model_data = []
    for model in models:
        model_json = model.booster_.dump_model()
        model_data.append(model_json)
    
    with open(MODEL_JSON, 'w') as f:
        json.dump(model_data, f, indent=2)
    
    print(f"✅ 軽量版モデル保存完了: {MODEL_JSON}")

def main():
    print("🧪 AtCoder AHC048 軽量版学習システム（動作確認用）")
    print("=" * 60)
    
    if not os.path.exists(INPUT_DIR):
        print(f"❌ エラー: {INPUT_DIR} ディレクトリが見つかりません")
        return
    
    if not os.path.exists(SOLVER_BIN):
        print(f"❌ エラー: {SOLVER_BIN} が見つかりません")
        return
    
    start_time = time.time()
    
    X, params, scores = create_lite_dataset()
    if X is None:
        return
    
    print(f"📊 特徴量次元: {X.shape[1]}")
    print(f"📊 データ数: {X.shape[0]}")
    
    models = train_lite_models(X, params)
    save_lite_models(models)
    
    elapsed = time.time() - start_time
    print(f"⏱️ 軽量版学習完了: {elapsed:.1f}秒")
    print(f"🧪 テスト用{MODEL_JSON}が生成されました！")
    
    print("\n✅ 動作確認成功！次は本格版:")
    print("   python train_params.py")

if __name__ == "__main__":
    main() 