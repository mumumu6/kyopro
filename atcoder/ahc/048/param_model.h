#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

// 🚀 高度機械学習による自動パラメータ最適化
// AtCoder AHC048 用 - アンサンブル対応版
namespace ml_params {

constexpr int FEATURE_SIZE = 22;  // 特徴量次元数
constexpr bool ENSEMBLE_MODE = true;

// ========== アンサンブル予測関数群 ==========

inline double predict_0_0(const std::array<double, FEATURE_SIZE>& features) {
    return 1.000000;
}

inline double predict_0_1(const std::array<double, FEATURE_SIZE>& features) {
    return 1.000000;
}

inline double predict_0_2(const std::array<double, FEATURE_SIZE>& features) {
    return 1.000000;
}
inline double predict_0(const std::array<double, FEATURE_SIZE>& features) {
    // アンサンブル予測（平均）
    double sum = predict_0_0(features) + predict_0_1(features) + predict_0_2(features);
    return sum / 3;
}

inline double predict_1_0(const std::array<double, FEATURE_SIZE>& features) {
    return 4.000000;
}

inline double predict_1_1(const std::array<double, FEATURE_SIZE>& features) {
    return 4.000000;
}

inline double predict_1_2(const std::array<double, FEATURE_SIZE>& features) {
    return 4.000000;
}
inline double predict_1(const std::array<double, FEATURE_SIZE>& features) {
    // アンサンブル予測（平均）
    double sum = predict_1_0(features) + predict_1_1(features) + predict_1_2(features);
    return sum / 3;
}

inline double predict_2_0(const std::array<double, FEATURE_SIZE>& features) {
    return 0.000000;
}

inline double predict_2_1(const std::array<double, FEATURE_SIZE>& features) {
    return 0.000000;
}

inline double predict_2_2(const std::array<double, FEATURE_SIZE>& features) {
    return 0.000000;
}
inline double predict_2(const std::array<double, FEATURE_SIZE>& features) {
    // アンサンブル予測（平均）
    double sum = predict_2_0(features) + predict_2_1(features) + predict_2_2(features);
    return sum / 3;
}
inline std::array<int, 3> predict_all_params(const std::array<double, FEATURE_SIZE>& features) {
    // 各パラメータの予測値を取得
    double pred_L = predict_0(features);
    double pred_B = predict_1(features);
    double pred_P = predict_2(features);
    
    // ソフトクリッピング＆適応的調整
    int L = std::clamp(static_cast<int>(std::round(pred_L)), 1, 5);
    int B = std::clamp(static_cast<int>(std::round(pred_B)), 4, 24);
    int P = std::clamp(static_cast<int>(std::round(pred_P)), 0, 4);
    
    // 論理的整合性チェック
    if (L >= 4 && B > 20) {
        B = std::min(B, 16);  // 高先読みでは探索幅を抑制
    }
    if (L <= 2 && B < 8) {
        B = std::max(B, 8);   // 低先読みでは最低限の探索幅確保
    }
    
    // ビーム幅の妥当性調整
    const std::array<int, 7> valid_beams = {4, 6, 8, 12, 16, 20, 24};
    auto it = std::lower_bound(valid_beams.begin(), valid_beams.end(), B);
    if (it != valid_beams.end()) {
        B = *it;
    } else {
        B = valid_beams.back();
    }
    
    return {L, B, P};
}

inline std::array<int, 3> predict_params_with_fallback(
    const std::array<double, FEATURE_SIZE>& features,
    int fallback_L = 2, int fallback_B = 8, int fallback_P = 0
) {
    try {
        return predict_all_params(features);
    } catch (...) {
        // フォールバック
        return {fallback_L, fallback_B, fallback_P};
    }
}
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
}
// ========== デバッグ&統計関数 ==========
inline void print_prediction_debug(const std::array<double, FEATURE_SIZE>& features, 
                                  const std::array<int, 3>& params) {
#ifdef DEBUG_ML_PREDICTION
    std::cerr << "🤖 ML予測デバッグ: ";
    std::cerr << "L=" << params[0] << ", B=" << params[1] << ", P=" << params[2];
    std::cerr << " (特徴量=" << features[0] << "," << features[1] << "," << features[2] << "," << features[3] << ")\n";
#endif
}

inline double calculate_prediction_confidence(const std::array<double, FEATURE_SIZE>& features) {
    // 予測信頼度を簡易計算（0-1の範囲）
    double complexity = features[0] * features[1] / (features[2] + 1.0);
    return std::clamp(1.0 / (1.0 + complexity * 0.001), 0.1, 0.9);
}
}  // namespace ml_params

// ========== 使用例 ==========
// auto features = ml_params::extract_features(K, H, T, D, own_colors, target_colors);
// auto [L, B, P] = ml_params::predict_all_params(features);
// ml_params::print_prediction_debug(features, {L, B, P});

// ========== コンパイル推奨オプション ==========
// g++ -O2 -march=native -DNDEBUG -std=c++20
