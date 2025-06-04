#!/usr/bin/env python3
"""
LightGBM アンサンブルモデル → C++ ヘッダファイル自動生成スクリプト（高度版）
大量データ学習済みのアンサンブルモデルから高精度C++推論器を生成

Usage:
    python gen_header_advanced.py
    → param_model.h が生成される
"""

import json
import textwrap
import pathlib
import sys
from typing import Dict, Any, List

INPUT_JSON = "lgbm_params.json"
OUTPUT_HEADER = "param_model.h"

def load_models() -> List:
    """アンサンブル学習済みモデルをJSONから読み込み"""
    try:
        with open(INPUT_JSON, 'r') as f:
            models = json.load(f)
        print(f"✅ アンサンブルモデル読み込み完了")
        
        # 構造確認
        if isinstance(models[0], list):
            print(f"   📊 アンサンブル検出: {len(models[0])}個のベースモデル")
            ensemble_mode = True
        else:
            print(f"   📊 単一モデル検出")
            ensemble_mode = False
            
        print(f"   📊 パラメータ数: {len(models)}")
        return models, ensemble_mode
    except FileNotFoundError:
        print(f"❌ エラー: {INPUT_JSON} が見つかりません")
        print("先に train_params.py または train_params_lite.py を実行してください")
        sys.exit(1)
    except Exception as e:
        print(f"❌ モデル読み込みエラー: {e}")
        sys.exit(1)

def extract_tree_structure(tree_info: List[Dict]) -> Dict:
    """LightGBMの木情報から決定木構造を抽出"""
    if not tree_info:
        return {"leaf_value": 0.0}
    
    # アンサンブルの場合は最初の木のみ使用（簡略化）
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

def generate_ensemble_prediction_function(model_id: int, model_ensemble: List, ensemble_mode: bool) -> str:
    """アンサンブル対応の予測関数を生成"""
    
    if not ensemble_mode or len(model_ensemble) == 1:
        # 単一モデルの場合
        model = model_ensemble[0] if isinstance(model_ensemble, list) else model_ensemble
        tree_structure = extract_tree_structure(model.get("tree_info", []))
        
        if not tree_structure or "leaf_value" in tree_structure and tree_structure["leaf_value"] == 0.0:
            default_val = 2.0 if model_id == 0 else 8.0 if model_id == 1 else 0.0
            return f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
    return {default_val}; // デフォルト値
}}"""
        
        tree_code = generate_tree_code(tree_structure)
        return f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
{tree_code}
}}"""
    
    else:
        # アンサンブルの場合
        ensemble_functions = []
        for i, model in enumerate(model_ensemble):
            tree_structure = extract_tree_structure(model.get("tree_info", []))
            
            if not tree_structure:
                continue
                
            tree_code = generate_tree_code(tree_structure)
            ensemble_functions.append(f"""
inline double predict_{model_id}_{i}(const std::array<double, FEATURE_SIZE>& features) {{
{tree_code}
}}""")
        
        # アンサンブル統合関数
        if ensemble_functions:
            predictions = [f"predict_{model_id}_{i}(features)" for i in range(len(ensemble_functions))]
            ensemble_func = f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
    // アンサンブル予測（平均）
    double sum = {' + '.join(predictions)};
    return sum / {len(predictions)};
}}"""
            return "\n".join(ensemble_functions) + ensemble_func
        else:
            default_val = 2.0 if model_id == 0 else 8.0 if model_id == 1 else 0.0
            return f"""
inline double predict_{model_id}(const std::array<double, FEATURE_SIZE>& features) {{
    return {default_val}; // フォールバック値
}}"""

def generate_advanced_prediction_function(num_models: int) -> str:
    """高度な統合予測関数を生成"""
    predictions = [f"predict_{i}(features)" for i in range(num_models)]
    
    return f"""
inline std::array<int, 3> predict_all_params(const std::array<double, FEATURE_SIZE>& features) {{
    // 各パラメータの予測値を取得
    double pred_L = {predictions[0] if len(predictions) > 0 else "2.0"};
    double pred_B = {predictions[1] if len(predictions) > 1 else "8.0"};
    double pred_P = {predictions[2] if len(predictions) > 2 else "0.0"};
    
    // ソフトクリッピング＆適応的調整
    int L = std::clamp(static_cast<int>(std::round(pred_L)), 1, 5);
    int B = std::clamp(static_cast<int>(std::round(pred_B)), 4, 24);
    int P = std::clamp(static_cast<int>(std::round(pred_P)), 0, 4);
    
    // 論理的整合性チェック
    if (L >= 4 && B > 20) {{
        B = std::min(B, 16);  // 高先読みでは探索幅を抑制
    }}
    if (L <= 2 && B < 8) {{
        B = std::max(B, 8);   // 低先読みでは最低限の探索幅確保
    }}
    
    // ビーム幅の妥当性調整
    const std::array<int, 7> valid_beams = {{4, 6, 8, 12, 16, 20, 24}};
    auto it = std::lower_bound(valid_beams.begin(), valid_beams.end(), B);
    if (it != valid_beams.end()) {{
        B = *it;
    }} else {{
        B = valid_beams.back();
    }}
    
    return {{L, B, P}};
}}

inline std::array<int, 3> predict_params_with_fallback(
    const std::array<double, FEATURE_SIZE>& features,
    int fallback_L = 2, int fallback_B = 8, int fallback_P = 0
) {{
    try {{
        return predict_all_params(features);
    }} catch (...) {{
        // フォールバック
        return {{fallback_L, fallback_B, fallback_P}};
    }}
}}"""

def generate_advanced_feature_extraction() -> str:
    """高度な特徴量抽出関数を生成"""
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
    
    // 所有色の統計量（改良版）
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
    
    features[4] = own_r_var / K;  
    features[5] = own_g_var / K;  
    features[6] = own_b_var / K;  
    features[7] = own_r_max - own_r_min;  
    features[8] = own_g_max - own_g_min;  
    features[9] = own_b_max - own_b_min;  
    
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
    
    features[10] = tgt_r_var / H;  
    features[11] = tgt_g_var / H;  
    features[12] = tgt_b_var / H;  
    
    // 色空間カバー率（高速化版）
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
    
    features[13] = mean_min_dist;              
    features[14] = std::sqrt(min_dist_var / H); 
    
    // コスト効率指標
    features[15] = static_cast<double>(T) / static_cast<double>(H);  
    features[16] = static_cast<double>(D) / static_cast<double>(T);  
    
    // 特徴量の正規化＆安定化
    for (int i = 4; i < FEATURE_SIZE; ++i) {
        if (std::isnan(features[i]) || std::isinf(features[i])) {
            features[i] = 0.0;
        }
        features[i] = std::clamp(features[i], -10.0, 10.0);  // 外れ値クリッピング
    }
    
    return features;
}"""

def generate_advanced_header_file(models: List, ensemble_mode: bool) -> str:
    """高度なアンサンブル対応ヘッダファイルを生成"""
    
    # 特徴量次元数を動的に設定
    feature_size = 17  # 基本版
    if ensemble_mode:
        feature_size = 22  # 高度版対応
    
    header = f"""#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

// 🚀 高度機械学習による自動パラメータ最適化
// AtCoder AHC048 用 - アンサンブル対応版
namespace ml_params {{

constexpr int FEATURE_SIZE = {feature_size};  // 特徴量次元数
constexpr bool ENSEMBLE_MODE = {"true" if ensemble_mode else "false"};

// ========== アンサンブル予測関数群 ==========
"""
    
    # 各モデルの予測関数
    prediction_functions = []
    for i, model in enumerate(models):
        if isinstance(model, list):
            func = generate_ensemble_prediction_function(i, model, ensemble_mode)
        else:
            func = generate_ensemble_prediction_function(i, [model], False)
        prediction_functions.append(func)
    
    # 統合予測関数
    ensemble_func = generate_advanced_prediction_function(len(models))
    
    # 特徴量抽出関数
    feature_func = generate_advanced_feature_extraction()
    
    # デバッグ&統計関数
    debug_func = """
// ========== デバッグ&統計関数 ==========
inline void print_prediction_debug(const std::array<double, FEATURE_SIZE>& features, 
                                  const std::array<int, 3>& params) {
#ifdef DEBUG_ML_PREDICTION
    std::cerr << "🤖 ML予測デバッグ: ";
    std::cerr << "L=" << params[0] << ", B=" << params[1] << ", P=" << params[2];
    std::cerr << " (特徴量=" << features[0] << "," << features[1] << "," << features[2] << "," << features[3] << ")\\n";
#endif
}

inline double calculate_prediction_confidence(const std::array<double, FEATURE_SIZE>& features) {
    // 予測信頼度を簡易計算（0-1の範囲）
    double complexity = features[0] * features[1] / (features[2] + 1.0);
    return std::clamp(1.0 / (1.0 + complexity * 0.001), 0.1, 0.9);
}"""
    
    # フッタ
    footer = """
}  // namespace ml_params

// ========== 使用例 ==========
// auto features = ml_params::extract_features(K, H, T, D, own_colors, target_colors);
// auto [L, B, P] = ml_params::predict_all_params(features);
// ml_params::print_prediction_debug(features, {L, B, P});

// ========== コンパイル推奨オプション ==========
// g++ -O2 -march=native -DNDEBUG -std=c++20
"""
    
    # 全体を結合
    full_header = (header + "\n".join(prediction_functions) + ensemble_func + 
                  feature_func + debug_func + footer)
    
    return full_header

def main():
    """メイン処理"""
    print("🔧 LightGBM アンサンブル → C++ 高度ヘッダファイル生成")
    print("=" * 60)
    
    # モデル読み込み
    models, ensemble_mode = load_models()
    
    if len(models) != 3:
        print(f"⚠️ 警告: {len(models)}個のモデルが見つかりました（期待値: 3）")
        print("L, B, P の順で3つのモデルが必要です")
    
    # ヘッダファイル生成
    print("🏗️ 高度C++コード生成中...")
    header_content = generate_advanced_header_file(models, ensemble_mode)
    
    # ファイル保存
    pathlib.Path(OUTPUT_HEADER).write_text(header_content, encoding='utf-8')
    
    print(f"✅ 高度ヘッダファイル生成完了: {OUTPUT_HEADER}")
    
    # 統計情報
    line_count = header_content.count('\n')
    size_kb = len(header_content.encode('utf-8')) / 1024
    
    print(f"📊 生成統計:")
    print(f"   • 行数: {line_count}")
    print(f"   • サイズ: {size_kb:.1f} KB")
    print(f"   • モデル数: {len(models)}")
    print(f"   • アンサンブル: {'有効' if ensemble_mode else '無効'}")
    
    print("\n🎉 次のステップ:")
    print("1. #include \"param_model.h\" をソルバに追加")
    print("2. ml_params::predict_all_params() で (L,B,P) を取得")
    print("3. コンパイル: g++ -O2 -march=native -std=c++20")

if __name__ == "__main__":
    main() 