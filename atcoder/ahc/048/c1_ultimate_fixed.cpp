#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define spa " "
#define el '\n'

// 汎用的な最適化設定（どのCPUでも動作）
#pragma GCC optimize("O3,unroll-loops")

struct Paint {
    float red, green, blue;
    ll count = 1;
    Paint(float r = 0, float g = 0, float b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
    
    // 高速色差計算（sqrt回避）
    inline float distance_sq_to(const Paint& other) const {
        float dr = red - other.red;
        float dg = green - other.green;
        float db = blue - other.blue;
        return dr * dr + dg * dg + db * db;
    }
    
    // HSV色空間での類似度計算
    tuple<float, float, float> to_hsv() const {
        float max_val = max({red, green, blue});
        float min_val = min({red, green, blue});
        float delta = max_val - min_val;
        
        float h = 0, s = 0, v = max_val;
        if (delta > 1e-6f) {
            s = delta / max_val;
            if (max_val == red) h = (green - blue) / delta;
            else if (max_val == green) h = 2.0f + (blue - red) / delta;
            else h = 4.0f + (red - green) / delta;
            h *= 60.0f;
            if (h < 0) h += 360.0f;
        }
        return make_tuple(h, s, v);
    }
};

// 高速ビームサーチエンジン（安定版）
class ColorOptimizer {
private:
    vector<Paint> base_colors;
    vector<Paint> targets;
    ll k, h, max_ops, disposal_cost;
    
    // 色特徴量分析
    vector<vector<float>> color_features; // RGB + HSV + Lab色空間
    vector<ll> optimal_color_set; // 最適色セット
    
    // ビームサーチ用構造体（メモリ効率重視）
    struct State {
        Paint current_color;
        ll target_idx;
        float total_cost;
        float heuristic_value;
        
        State() : current_color(0, 0, 0, 0), target_idx(0), total_cost(0), heuristic_value(1e9f) {}
        
        bool operator<(const State& other) const {
            return heuristic_value > other.heuristic_value; // priority_queueは大きい順なので逆転
        }
    };
    
    // 乱数生成器
    mt19937 rng;
    uniform_real_distribution<float> uniform_dist;
    
public:
    ColorOptimizer(const vector<Paint>& colors, const vector<Paint>& tgts, ll ops, ll cost)
        : base_colors(colors), targets(tgts), k(colors.size()), h(tgts.size()),
          max_ops(ops), disposal_cost(cost), rng(42), uniform_dist(0.0f, 1.0f) {
        
        analyze_color_features();
        select_optimal_color_set();
    }
    
private:
    // AI風色特徴量抽出
    void analyze_color_features() {
        color_features.assign(k, vector<float>(9)); // RGB + HSV + Lab近似
        
        rep(i, k) {
            const auto& color = base_colors[i];
            auto& features = color_features[i];
            
            // RGB特徴量
            features[0] = color.red;
            features[1] = color.green;
            features[2] = color.blue;
            
            // HSV特徴量
            auto [h, s, v] = color.to_hsv();
            features[3] = h / 360.0f;
            features[4] = s;
            features[5] = v;
            
            // Lab色空間近似特徴量
            float l = 0.299f * color.red + 0.587f * color.green + 0.114f * color.blue;
            float a = 0.5f * (color.red - color.green);
            float b = 0.25f * (color.red + color.green - 2.0f * color.blue);
            features[6] = l;
            features[7] = a;
            features[8] = b;
        }
    }
    
    // 最適色セット選択（k-means++的手法）
    void select_optimal_color_set() {
        vector<bool> selected(k, false);
        optimal_color_set.clear();
        
        // 各ターゲットに最も近い色を最優先
        rep(i, h) {
            ll best_color = 0;
            float best_dist = 1e9f;
            rep(j, k) {
                if (selected[j]) continue;
                float dist = base_colors[j].distance_sq_to(targets[i]);
                if (dist < best_dist) {
                    best_dist = dist;
                    best_color = j;
                }
            }
            if (!selected[best_color]) {
                selected[best_color] = true;
                optimal_color_set.push_back(best_color);
            }
        }
        
        // 多様性を考慮した追加色選択
        ll max_colors = min(k, 10LL); // 少し減らして安定性重視
        while (optimal_color_set.size() < max_colors) {
            ll best_color = 0;
            float best_diversity = -1;
            
            rep(i, k) {
                if (selected[i]) continue;
                
                float min_dist = 1e9f;
                for (ll j : optimal_color_set) {
                    float dist = 0;
                    rep(f, 9) {
                        float diff = color_features[i][f] - color_features[j][f];
                        dist += diff * diff;
                    }
                    min_dist = min(min_dist, dist);
                }
                
                if (min_dist > best_diversity) {
                    best_diversity = min_dist;
                    best_color = i;
                }
            }
            
            selected[best_color] = true;
            optimal_color_set.push_back(best_color);
        }
    }
    
    // 高速色混合計算
    struct MixCandidate {
        vector<ll> colors;
        vector<ll> amounts;
        ll use_current;
        Paint result;
        float error;
        
        MixCandidate() : error(1e9f), use_current(0) {}
    };
    
    MixCandidate find_best_mix_fast(const Paint& current, ll target_idx) {
        MixCandidate best;
        const Paint& target = targets[target_idx];
        
        ll color_limit = min((ll)optimal_color_set.size(), 6LL);
        ll amount_limit = min(max_ops / 3, 4LL);
        
        // 2色混合から3色混合まで段階的に探索（4色は重すぎるので削除）
        for (ll num_colors = 2; num_colors <= min(3LL, color_limit); ++num_colors) {
            vector<ll> indices(num_colors);
            rep(i, num_colors) indices[i] = i;
            
            do {
                vector<ll> color_ids;
                rep(i, num_colors) color_ids.push_back(optimal_color_set[indices[i]]);
                
                // 再帰を避けてネストループで実装（メモリ効率向上）
                vector<ll> amounts(num_colors);
                
                function<void(ll)> enumerate = [&](ll pos) {
                    if (pos == num_colors) {
                        ll total_amount = 0;
                        for (ll a : amounts) total_amount += a;
                        if (total_amount == 0 || total_amount > max_ops) return;
                        
                        // 現在色の使用量を簡素化
                        for (ll use_current = 0; use_current <= min(current.count, 3LL); ++use_current) {
                            ll total = total_amount + use_current;
                            if (total == 0) continue;
                            
                            float new_r = current.red * use_current;
                            float new_g = current.green * use_current;
                            float new_b = current.blue * use_current;
                            
                            rep(i, num_colors) {
                                new_r += base_colors[color_ids[i]].red * amounts[i];
                                new_g += base_colors[color_ids[i]].green * amounts[i];
                                new_b += base_colors[color_ids[i]].blue * amounts[i];
                            }
                            
                            new_r /= total;
                            new_g /= total;
                            new_b /= total;
                            
                            Paint result(new_r, new_g, new_b, total);
                            float color_error = sqrt(result.distance_sq_to(target)) * 10000.0f;
                            float cost_error = (total_amount + (current.count - use_current)) * disposal_cost;
                            float total_error = color_error + cost_error;
                            
                            if (total_error < best.error) {
                                best.error = total_error;
                                best.colors = color_ids;
                                best.amounts = amounts;
                                best.use_current = use_current;
                                best.result = result;
                            }
                        }
                        return;
                    }
                    
                    rep(a, amount_limit + 1) {
                        amounts[pos] = a;
                        enumerate(pos + 1);
                    }
                };
                
                enumerate(0);
                
            } while (next_permutation(indices.begin(), indices.end()) && 
                     indices.back() < color_limit);
        }
        
        return best;
    }
    
public:
    // メインの最適化エンジン（簡素化版）
    vector<MixCandidate> optimize_sequence() {
        vector<MixCandidate> results;
        Paint current_paint(0, 0, 0, 0);
        
        // グリーディ初期解
        rep(i, h) {
            auto mix = find_best_mix_fast(current_paint, i);
            results.push_back(mix);
            current_paint = mix.result;
        }
        
        // 簡単な局所改善（焼きなましの軽量版）
        ll iterations = 1000; // 軽量化
        rep(iter, iterations) {
            ll pos = rng() % h;
            
            Paint temp_current(0, 0, 0, 0);
            rep(j, pos) {
                temp_current = results[j].result;
            }
            
            auto new_mix = find_best_mix_fast(temp_current, pos);
            if (new_mix.error < results[pos].error) {
                results[pos] = new_mix;
                
                // 後続を更新
                Paint cascade_current = new_mix.result;
                reps(j, pos + 1, h) {
                    results[j] = find_best_mix_fast(cascade_current, j);
                    cascade_current = results[j].result;
                }
            }
        }
        
        return results;
    }
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k);
    vector<Paint> target_color(h);

    rep(i, k) {
        double r, g, b;
        cin >> r >> g >> b;
        own_color[i] = Paint(r, g, b);
    }
    rep(i, h) {
        double r, g, b;
        cin >> r >> g >> b;
        target_color[i] = Paint(r, g, b);
    }

    // パレット出力
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;

    // 最適化エンジン起動
    ColorOptimizer optimizer(own_color, target_color, c, d);
    auto results = optimizer.optimize_sequence();

    // 結果出力
    rep(i, h) {
        if (i >= results.size()) break; // 安全性チェック
        
        auto& mix = results[i];
        
        // 廃棄処理
        ll dispose_count = (i == 0) ? 0 : max(0LL, results[i-1].result.count - mix.use_current);
        rep(j, dispose_count) {
            cout << 3 << spa << 0 << spa << 0 << el;
        }
        
        // 色追加処理  
        rep(j, min((ll)mix.colors.size(), (ll)mix.amounts.size())) {
            rep(k, mix.amounts[j]) {
                cout << 1 << spa << 0 << spa << 0 << spa << mix.colors[j] << el;
            }
        }
        
        // 混合処理
        cout << 2 << spa << 0 << spa << 0 << el;
    }
    
    return 0;
} 