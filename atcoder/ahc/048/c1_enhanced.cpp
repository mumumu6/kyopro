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
#define spa " "
#define el '\n'

// 安全なコンパイラ最適化
#pragma GCC optimize("O3,unroll-loops")

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
    
    // 色距離計算
    double distance_to(const Paint& other) const {
        double dr = red - other.red;
        double dg = green - other.green;
        double db = blue - other.blue;
        return sqrt(dr * dr + dg * dg + db * db);
    }
};

// パレット解析クラス
class PaletteAnalyzer {
private:
    vector<Paint> base_colors;
    vector<Paint> targets;
    ll k, h;
    
public:
    PaletteAnalyzer(const vector<Paint>& colors, const vector<Paint>& tgts) 
        : base_colors(colors), targets(tgts), k(colors.size()), h(tgts.size()) {}
    
    // 各ターゲットに対する優先色を分析
    vector<vector<ll>> analyze_priority_colors() {
        vector<vector<ll>> priorities(h);
        
        rep(i, h) {
            vector<pair<double, ll>> distances;
            rep(j, k) {
                distances.emplace_back(base_colors[j].distance_to(targets[i]), j);
            }
            sort(distances.begin(), distances.end());
            
            // 上位8色を記録（多すぎると遅くなる）
            for(ll idx = 0; idx < min(k, 8LL); ++idx) {
                priorities[i].push_back(distances[idx].second);
            }
        }
        
        return priorities;
    }
    
    // グローバル重要色の選択
    vector<ll> select_global_important_colors() {
        vector<double> color_scores(k, 0);
        
        rep(i, h) {
            rep(j, k) {
                // 距離の逆数でスコア計算（近いほど高スコア）
                double dist = base_colors[j].distance_to(targets[i]);
                color_scores[j] += 1.0 / (1.0 + dist);
            }
        }
        
        vector<pair<double, ll>> scored_colors;
        rep(i, k) {
            scored_colors.emplace_back(color_scores[i], i);
        }
        sort(scored_colors.rbegin(), scored_colors.rend());
        
        vector<ll> important_colors;
        for(ll i = 0; i < min(k, 10LL); ++i) {
            important_colors.push_back(scored_colors[i].second);
        }
        
        return important_colors;
    }
};

// 拡張版メイン処理
class EnhancedColorMixer {
private:
    vector<Paint> own_color;
    vector<Paint> target_color;
    ll n, k, h, t, d, c;
    PaletteAnalyzer analyzer;
    
    mt19937 rng;
    
public:
    EnhancedColorMixer(ll n_, ll k_, ll h_, ll t_, ll d_, 
                      const vector<Paint>& own, const vector<Paint>& target)
        : n(n_), k(k_), h(h_), t(t_), d(d_), c((t/h)/2), 
          own_color(own), target_color(target), analyzer(own, target), rng(42) {}
    
    struct MixResult {
        ll usecount_a, usecount_b, usecount_c, usecount_d;
        ll use_id1, use_id2, use_id3, use_id4;
        ll dis_count;
        Paint new_color;
        double error;
        
        MixResult() : usecount_a(0), usecount_b(0), usecount_c(0), usecount_d(0),
                     use_id1(0), use_id2(1), use_id3(1), use_id4(1), dis_count(0),
                     new_color(0,0,0,0), error(1e9) {}
    };
    
    // 元のアルゴリズムベースの混合計算（高精度版）
    MixResult calculate_best_mix_original(const Paint& now_color, ll target_idx) {
        MixResult best;
        
        const Paint& tr = target_color[target_idx];
        auto squ = [](double x) { return x * x; };
        
        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;
        
        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) for (int c4 = c3 + 1; c4 < k; c4+=2) {
            const Paint &color1 = own_color[c1];
            const Paint &color2 = own_color[c2];
            const Paint &color3 = own_color[c3];
            const Paint &color4 = own_color[c4];

            ll use_now_color = -qq;
            rep(dis, 4) {
                use_now_color += (dis < rr ? qq + 1 : qq);

                rep(ca, 4) rep(cb, 4) rep(cc, 4) rep(cd, 3) {
                    if (ca + cb + cc + cd > c) continue;
                    if (ca + cb + cc + cd + use_now_color < 1) continue;
                    ll sum = ca + cb + cc + cd + use_now_color;

                    double r = (color1.red * ca + color2.red * cb + color3.red * cc + color4.red * cd +
                                now_color.red * use_now_color) / sum;
                    double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                color4.green * cd + now_color.green * use_now_color) / sum;
                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                color4.blue * cd + now_color.blue * use_now_color) / sum;

                    double n_error = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                                    (ca + cb + cc + cd + now_color.count - use_now_color) * d;

                    if (n_error < best.error) {
                        best.error = n_error;
                        best.usecount_a = ca;
                        best.usecount_b = cb;
                        best.usecount_c = cc;
                        best.usecount_d = cd;
                        best.use_id1 = c1;
                        best.use_id2 = c2;
                        best.use_id3 = c3;
                        best.use_id4 = c4;
                        best.new_color = Paint(r, g, b, sum);
                        best.dis_count = now_color.count - use_now_color;
                    }
                }
            }
        }
        
        return best;
    }
    
    // パレット優先版（精度重視）
    MixResult calculate_best_mix_smart(const Paint& now_color, ll target_idx) {
        MixResult best;
        
        const Paint& tr = target_color[target_idx];
        auto squ = [](double x) { return x * x; };
        
        // パレット分析結果を活用
        auto important_colors = analyzer.select_global_important_colors();
        auto priority_colors = analyzer.analyze_priority_colors();
        
        // まず重要色の組み合わせを試す
        ll limit_colors = min((ll)important_colors.size(), 8LL);
        
        rep(i1, limit_colors) {
            ll c1 = important_colors[i1];
            reps(i2, i1 + 1, limit_colors) {
                ll c2 = important_colors[i2];
                reps(i3, i2 + 1, limit_colors) {
                    ll c3 = important_colors[i3];
                    reps(i4, i3 + 1, limit_colors) {
                        ll c4 = important_colors[i4];
                        
                        const Paint &color1 = own_color[c1];
                        const Paint &color2 = own_color[c2];
                        const Paint &color3 = own_color[c3];
                        const Paint &color4 = own_color[c4];

                        ll qq = now_color.count / 4;
                        ll rr = now_color.count % 4;
                        ll use_now_color = -qq;
                        
                        rep(dis, 4) {
                            use_now_color += (dis < rr ? qq + 1 : qq);

                            rep(ca, 4) rep(cb, 4) rep(cc, 4) rep(cd, 3) {
                                if (ca + cb + cc + cd > c) continue;
                                if (ca + cb + cc + cd + use_now_color < 1) continue;
                                ll sum = ca + cb + cc + cd + use_now_color;

                                double r = (color1.red * ca + color2.red * cb + color3.red * cc + color4.red * cd +
                                            now_color.red * use_now_color) / sum;
                                double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                            color4.green * cd + now_color.green * use_now_color) / sum;
                                double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                            color4.blue * cd + now_color.blue * use_now_color) / sum;

                                double n_error = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                                                (ca + cb + cc + cd + now_color.count - use_now_color) * d;

                                if (n_error < best.error) {
                                    best.error = n_error;
                                    best.usecount_a = ca;
                                    best.usecount_b = cb;
                                    best.usecount_c = cc;
                                    best.usecount_d = cd;
                                    best.use_id1 = c1;
                                    best.use_id2 = c2;
                                    best.use_id3 = c3;
                                    best.use_id4 = c4;
                                    best.new_color = Paint(r, g, b, sum);
                                    best.dis_count = now_color.count - use_now_color;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return best;
    }
    
    // メイン処理
    vector<MixResult> solve() {
        vector<MixResult> results;
        Paint now_color(0, 0, 0, 0);
        
        // 初期解をスマート版で計算
        rep(hi, h) {
            auto smart_result = calculate_best_mix_smart(now_color, hi);
            auto original_result = calculate_best_mix_original(now_color, hi);
            
            // より良い方を採用
            if (smart_result.error < original_result.error) {
                results.push_back(smart_result);
                now_color = smart_result.new_color;
            } else {
                results.push_back(original_result);
                now_color = original_result.new_color;
            }
            now_color.count--; // 元のロジックに合わせる
        }
        
        // 軽量な局所改善
        ll improvement_rounds = 3;
        rep(round, improvement_rounds) {
            Paint temp_color(0, 0, 0, 0);
            
            rep(hi, h) {
                auto improved = calculate_best_mix_smart(temp_color, hi);
                if (improved.error < results[hi].error) {
                    results[hi] = improved;
                }
                temp_color = results[hi].new_color;
                temp_color.count--;
            }
        }
        
        return results;
    }
    
    void output_solution() {
        // パレット出力
        rep(ii, n) {
            rep(i, n - 1) cout << 0 << " ";
            cout << el;
        }
        rep(ii, n - 1) {
            rep(i, n) cout << 0 << " ";
            cout << el;
        }
        
        auto results = solve();
        
        // 解出力
        rep(hi, h) {
            if (hi >= results.size()) break;
            
            const auto& result = results[hi];
            
            rep(i, result.dis_count) { 
                cout << 3 << spa << 0 << spa << 0 << el; 
            }
            rep(i, result.usecount_a) 
                cout << 1 << spa << 0 << spa << 0 << spa << result.use_id1 << el;
            rep(i, result.usecount_b) 
                cout << 1 << spa << 0 << spa << 0 << spa << result.use_id2 << el;
            rep(i, result.usecount_c) 
                cout << 1 << spa << 0 << spa << 0 << spa << result.use_id3 << el;
            rep(i, result.usecount_d) 
                cout << 1 << spa << 0 << spa << 0 << spa << result.use_id4 << el;
            cout << 2 << spa << 0 << spa << 0 << el;
        }
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

    EnhancedColorMixer mixer(n, k, h, t, d, own_color, target_color);
    mixer.output_solution();
    
    return 0;
} 