#!/usr/bin/env python3
"""
LightGBM JSON → C++ ヘッダファイル自動生成スクリプト
lgbm_params.json から param_model.h を生成し、C++で高速推論を可能にする

Usage:
    python gen_header.py
    → param_model.h が生成される
"""

import json
import textwrap
import pathlib
import sys
from typing import Dict, Any, List

INPUT_JSON = "lgbm_params.json"
OUTPUT_HEADER = "param_model.h"

def load_models() -> List[Dict]:
    """学習済みモデルをJSONから読み込み"""
    try:
        with open(INPUT_JSON, 'r') as f:
            models = json.load(f)
        print(f"✅ モデル読み込み完了: {len(models)} モデル")
        return models
    except FileNotFoundError:
        print(f"❌ エラー: {INPUT_JSON} が見つかりません")
        print("先に train_params.py を実行してください")
        sys.exit(1)
    except Exception as e:
        print(f"❌ モデル読み込みエラー: {e}")
        sys.exit(1)

def extract_tree_structure(tree_info: List[Dict]) -> Dict:
    """LightGBMの木情報から決定木構造を抽出"""
    if not tree_info:
        return {"leaf_value": 0.0}  # デフォルト値
    
    # 最初の木のみ使用（シンプル化）
    return tree_info[0]["tree_structure"]

def generate_tree_code(node: Dict, indent: int = 4) -> str:
    """決定木ノードから再帰的にC++コードを生成"""
    ind = " " * indent
    
    # リーフノード
    if "leaf_value" in node:
        return f"{ind}return {node['leaf_value']:.6f};"
    
    # 内部ノード
    feature_id = node.get("split_feature", 0)
    threshold = node.get("threshold", 0.0)
    
    left_code = generate_tree_code(node.get("left_child", {}), indent + 4)
    right_code = generate_tree_code(node.get("right_child", {}), indent + 4)
    
    return textwrap.dedent(f"""
    {ind}if (features[{feature_id}] <= {threshold:.6f}) {{
    {left_code}
    {ind}}} else {{
    {right_code}
    {ind}}}""").strip()

def generate_prediction_function(model_id: int, model: Dict) -> str:
    """1つのモデルに対する予測関数を生成"""
    tree_structure = extract_tree_structure(model.get("tree_info", []))
    
    if not tree_structure:
        print(f"⚠️ モデル{model_id}: 木構造が見つかりません。デフォルト値を使用します。")
        return f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
    return {2.0 if model_id == 0 else 8.0 if model_id == 1 else 0.0}; // デフォルト値
}}"""
    
    tree_code = generate_tree_code(tree_structure)
    
    return f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
{tree_code}
}}"""

def generate_ensemble_function(num_models: int) -> str:
    """全モデルを統合する予測関数を生成"""
    predictions = [f"predict_{i}(features)" for i in range(num_models)]
    
    return f"""
inline std::array<int, 3> predict_all_params(const std::array<double, FEATURE_SIZE>& features) {{
    // 各パラメータの予測値を取得
    double pred_L = {predictions[0] if len(predictions) > 0 else "2.0"};
    double pred_B = {predictions[1] if len(predictions) > 1 else "8.0"};
    double pred_P = {predictions[2] if len(predictions) > 2 else "0.0"};
    
    // 整数値に変換＆範囲制限
    int L = std::clamp(static_cast<int>(std::round(pred_L)), 1, 5);
    int B = std::clamp(static_cast<int>(std::round(pred_B)), 4, 20);
    int P = std::clamp(static_cast<int>(std::round(pred_P)), 0, 4);
    
    return {{L, B, P}};
}}"""

def generate_feature_extraction() -> str:
    """特徴量抽出関数を生成"""
    return """
inline std::array<double, FEATURE_SIZE> extract_features(
    int K, int H, int T, int D,
    const std::vector<std::array<double, 3>>& own_colors,
    const std::vector<std::array<double, 3>>& target_colors
) {
    std::array<double, FEATURE_SIZE> features = {};
    
    // 基本特徴量
    features[0] = static_cast<double>(K);
    features[1] = static_cast<double>(H);
    features[2] = static_cast<double>(T);
    features[3] = static_cast<double>(D);
    
    // 所有色の統計量
    double own_r_sum = 0, own_g_sum = 0, own_b_sum = 0;
    for (const auto& c : own_colors) {
        own_r_sum += c[0];
        own_g_sum += c[1];
        own_b_sum += c[2];
    }
    double own_r_mean = own_r_sum / K;
    double own_g_mean = own_g_sum / K;
    double own_b_mean = own_b_sum / K;
    
    double own_r_var = 0, own_g_var = 0, own_b_var = 0;
    double own_r_min = 1.0, own_r_max = 0.0;
    double own_g_min = 1.0, own_g_max = 0.0;
    double own_b_min = 1.0, own_b_max = 0.0;
    
    for (const auto& c : own_colors) {
        own_r_var += (c[0] - own_r_mean) * (c[0] - own_r_mean);
        own_g_var += (c[1] - own_g_mean) * (c[1] - own_g_mean);
        own_b_var += (c[2] - own_b_mean) * (c[2] - own_b_mean);
        
        own_r_min = std::min(own_r_min, c[0]);
        own_r_max = std::max(own_r_max, c[0]);
        own_g_min = std::min(own_g_min, c[1]);
        own_g_max = std::max(own_g_max, c[1]);
        own_b_min = std::min(own_b_min, c[2]);
        own_b_max = std::max(own_b_max, c[2]);
    }
    
    features[4] = own_r_var / K;  // R分散
    features[5] = own_g_var / K;  // G分散
    features[6] = own_b_var / K;  // B分散
    features[7] = own_r_max - own_r_min;  // R範囲
    features[8] = own_g_max - own_g_min;  // G範囲
    features[9] = own_b_max - own_b_min;  // B範囲
    
    // ターゲット色の統計量
    double tgt_r_sum = 0, tgt_g_sum = 0, tgt_b_sum = 0;
    for (const auto& c : target_colors) {
        tgt_r_sum += c[0];
        tgt_g_sum += c[1];
        tgt_b_sum += c[2];
    }
    double tgt_r_mean = tgt_r_sum / H;
    double tgt_g_mean = tgt_g_sum / H;
    double tgt_b_mean = tgt_b_sum / H;
    
    double tgt_r_var = 0, tgt_g_var = 0, tgt_b_var = 0;
    for (const auto& c : target_colors) {
        tgt_r_var += (c[0] - tgt_r_mean) * (c[0] - tgt_r_mean);
        tgt_g_var += (c[1] - tgt_g_mean) * (c[1] - tgt_g_mean);
        tgt_b_var += (c[2] - tgt_b_mean) * (c[2] - tgt_b_mean);
    }
    
    features[10] = tgt_r_var / H;  // ターゲットR分散
    features[11] = tgt_g_var / H;  // ターゲットG分散
    features[12] = tgt_b_var / H;  // ターゲットB分散
    
    // 色空間カバー率
    double total_min_dist = 0.0;
    double min_dist_var = 0.0;
    
    for (const auto& tgt : target_colors) {
        double min_dist = 1e9;
        for (const auto& own : own_colors) {
            double dist = std::sqrt(
                (tgt[0] - own[0]) * (tgt[0] - own[0]) +
                (tgt[1] - own[1]) * (tgt[1] - own[1]) +
                (tgt[2] - own[2]) * (tgt[2] - own[2])
            );
            min_dist = std::min(min_dist, dist);
        }
        total_min_dist += min_dist;
    }
    
    double mean_min_dist = total_min_dist / H;
    
    for (const auto& tgt : target_colors) {
        double min_dist = 1e9;
        for (const auto& own : own_colors) {
            double dist = std::sqrt(
                (tgt[0] - own[0]) * (tgt[0] - own[0]) +
                (tgt[1] - own[1]) * (tgt[1] - own[1]) +
                (tgt[2] - own[2]) * (tgt[2] - own[2])
            );
            min_dist = std::min(min_dist, dist);
        }
        min_dist_var += (min_dist - mean_min_dist) * (min_dist - mean_min_dist);
    }
    
    features[13] = mean_min_dist;              // 平均最小距離
    features[14] = std::sqrt(min_dist_var / H); // 距離の標準偏差
    
    // コスト効率指標
    features[15] = static_cast<double>(T) / static_cast<double>(H);  // ターン/ターゲット比
    features[16] = static_cast<double>(D) / static_cast<double>(T);  // コスト密度
    
    return features;
}"""

def generate_header_file(models: List[Dict]) -> str:
    """完全なヘッダファイルを生成"""
    
    # ヘッダ部分
    header = """#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>

// 機械学習による自動パラメータ最適化
// AtCoder AHC048 用
namespace ml_params {

constexpr int FEATURE_SIZE = 17;  // 特徴量次元数
"""
    
    # 各モデルの予測関数
    prediction_functions = []
    for i, model in enumerate(models):
        func = generate_prediction_function(i, model)
        prediction_functions.append(func)
    
    # 統合予測関数
    ensemble_func = generate_ensemble_function(len(models))
    
    # 特徴量抽出関数
    feature_func = generate_feature_extraction()
    
    # フッタ
    footer = """
}  // namespace ml_params

// 使用例:
// auto features = ml_params::extract_features(K, H, T, D, own_colors, target_colors);
// auto [L, B, P] = ml_params::predict_all_params(features);
"""
    
    # 全体を結合
    full_header = header + "\n".join(prediction_functions) + ensemble_func + feature_func + footer
    
    return full_header

def main():
    """メイン処理"""
    print("🔧 LightGBM → C++ ヘッダファイル生成")
    print("=" * 50)
    
    # モデル読み込み
    models = load_models()
    
    if len(models) != 3:
        print(f"⚠️ 警告: {len(models)}個のモデルが見つかりました（期待値: 3）")
        print("L, B, P の順で3つのモデルが必要です")
    
    # ヘッダファイル生成
    print("🏗️ C++コード生成中...")
    header_content = generate_header_file(models)
    
    # ファイル保存
    pathlib.Path(OUTPUT_HEADER).write_text(header_content, encoding='utf-8')
    
    print(f"✅ ヘッダファイル生成完了: {OUTPUT_HEADER}")
    
    # 統計情報
    line_count = header_content.count('\n')
    size_kb = len(header_content.encode('utf-8')) / 1024
    
    print(f"📊 生成統計:")
    print(f"   • 行数: {line_count}")
    print(f"   • サイズ: {size_kb:.1f} KB")
    print(f"   • モデル数: {len(models)}")
    
    print("\n🎉 次のステップ:")
    print("1. #include \"param_model.h\" をソルバに追加")
    print("2. ml_params::predict_all_params() で (L,B,P) を取得")
    print("3. コンパイル時に -O2 -march=native を推奨")

if __name__ == "__main__":
    main() 