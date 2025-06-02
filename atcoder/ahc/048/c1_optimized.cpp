#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
    
    // 色差計算の高速化
    inline double distance_to(const Paint& other) const {
        double dr = red - other.red;
        double dg = green - other.green;
        double db = blue - other.blue;
        return sqrt(dr * dr + dg * dg + db * db);
    }
};

// 高効率パレット管理システム
class SmartPalette {
private:
    vector<Paint> base_colors;
    vector<Paint> targets;
    vector<vector<double>> color_distances; // 事前計算した色距離
    vector<ll> priority_colors; // 重要度順の色インデックス
    ll k, h, max_operations, disposal_cost;
    
    // 乱数生成器（焼きなまし用）
    mt19937 rng;
    uniform_real_distribution<double> uniform_dist;
    
public:
    SmartPalette(const vector<Paint>& colors, const vector<Paint>& tgts, ll ops, ll cost) 
        : base_colors(colors), targets(tgts), k(colors.size()), h(tgts.size()), 
          max_operations(ops), disposal_cost(cost), rng(42), uniform_dist(0.0, 1.0) {
        
        precompute_distances();
        analyze_color_importance();
    }
    
private:
    // 色距離の事前計算で高速化
    void precompute_distances() {
        color_distances.assign(k, vector<double>(h));
        rep(i, k) rep(j, h) {
            color_distances[i][j] = base_colors[i].distance_to(targets[j]);
        }
    }
    
    // 色の重要度分析（ターゲットとの類似度ベース）
    void analyze_color_importance() {
        vector<pair<double, ll>> color_scores;
        
        rep(i, k) {
            double total_score = 0;
            rep(j, h) {
                // 近い色ほど高スコア
                total_score += 1.0 / (1.0 + color_distances[i][j]);
            }
            color_scores.emplace_back(total_score, i);
        }
        
        sort(color_scores.rbegin(), color_scores.rend());
        priority_colors.clear();
        for(auto& p : color_scores) {
            priority_colors.push_back(p.second);
        }
    }
    
    // 効率的な色混合計算
    struct MixResult {
        vector<ll> color_ids;
        vector<ll> amounts;
        ll use_current;
        ll dispose_count;
        Paint result_color;
        double error;
        
        MixResult() : error(1e9) {}
    };
    
    MixResult calculate_best_mix(const Paint& current_color, ll target_idx) {
        MixResult best;
        
        // 限定的な探索空間で高速化
        ll search_colors = min(k, 8LL); // 上位8色のみ探索
        ll max_amount = min(max_operations / 2, 5LL); // 各色最大5まで
        
        // 現在の色の分割パターン
        ll current_parts = min(current_color.count, 4LL);
        
        rep(pattern, 1 << current_parts) {
            ll use_current = __builtin_popcount(pattern);
            ll dispose_current = current_color.count - use_current;
            
            // 3色混合を優先的に探索
            rep(i, search_colors) {
                ll c1 = priority_colors[i];
                reps(j, i + 1, search_colors) {
                    ll c2 = priority_colors[j];
                    reps(l, j + 1, search_colors) {
                        ll c3 = priority_colors[l];
                        
                        rep(a1, max_amount + 1) rep(a2, max_amount + 1) rep(a3, max_amount + 1) {
                            if(a1 + a2 + a3 + use_current == 0) continue;
                            if(a1 + a2 + a3 > max_operations) continue;
                            
                            ll total = a1 + a2 + a3 + use_current;
                            
                            double new_r = (base_colors[c1].red * a1 + 
                                          base_colors[c2].red * a2 + 
                                          base_colors[c3].red * a3 + 
                                          current_color.red * use_current) / total;
                            double new_g = (base_colors[c1].green * a1 + 
                                          base_colors[c2].green * a2 + 
                                          base_colors[c3].green * a3 + 
                                          current_color.green * use_current) / total;
                            double new_b = (base_colors[c1].blue * a1 + 
                                          base_colors[c2].blue * a2 + 
                                          base_colors[c3].blue * a3 + 
                                          current_color.blue * use_current) / total;
                            
                            Paint new_color(new_r, new_g, new_b, total);
                            
                            double color_error = new_color.distance_to(targets[target_idx]) * 10000.0;
                            double cost_penalty = (a1 + a2 + a3 + dispose_current) * disposal_cost;
                            double total_error = color_error + cost_penalty;
                            
                            if(total_error < best.error) {
                                best.error = total_error;
                                best.color_ids = {c1, c2, c3};
                                best.amounts = {a1, a2, a3};
                                best.use_current = use_current;
                                best.dispose_count = dispose_current;
                                best.result_color = new_color;
                            }
                        }
                    }
                }
            }
        }
        
        return best;
    }
    
public:
    // 焼きなまし法による解の改善
    vector<MixResult> optimize_sequence() {
        vector<MixResult> results;
        Paint current(0, 0, 0, 0);
        
        // 初期解の構築
        rep(i, h) {
            auto mix = calculate_best_mix(current, i);
            results.push_back(mix);
            current = mix.result_color;
        }
        
        // 焼きなまし法で改善
        double temperature = 1000.0;
        double cooling_rate = 0.995;
        ll iterations = 10000;
        
        rep(iter, iterations) {
            // ランダムな位置の解を変更
            ll pos = rng() % h;
            
            // 現在位置での色を再計算
            Paint temp_current(0, 0, 0, 0);
            rep(j, pos) {
                temp_current = results[j].result_color;
            }
            
            auto old_mix = results[pos];
            auto new_mix = calculate_best_mix(temp_current, pos);
            
            // 後続の解への影響を計算
            double delta = new_mix.error - old_mix.error;
            
            // 受理判定
            if(delta < 0 || uniform_dist(rng) < exp(-delta / temperature)) {
                results[pos] = new_mix;
                
                // 後続の解を再計算
                Paint cascade_current = new_mix.result_color;
                reps(j, pos + 1, h) {
                    results[j] = calculate_best_mix(cascade_current, j);
                    cascade_current = results[j].result_color;
                }
            }
            
            temperature *= cooling_rate;
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

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

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

    // 高効率パレット最適化システムを使用
    SmartPalette palette(own_color, target_color, c, d);
    auto optimized_sequence = palette.optimize_sequence();

    // 最適化された結果を出力
    rep(i, h) {
        auto& mix = optimized_sequence[i];
        
        // 廃棄処理
        rep(j, mix.dispose_count) {
            cout << 3 << spa << 0 << spa << 0 << el;
        }
        
        // 色追加処理
        rep(j, mix.color_ids.size()) {
            rep(k, mix.amounts[j]) {
                cout << 1 << spa << 0 << spa << 0 << spa << mix.color_ids[j] << el;
            }
        }
        
        // 混合処理
        cout << 2 << spa << 0 << spa << 0 << el;
    }
    
    return 0;
} 