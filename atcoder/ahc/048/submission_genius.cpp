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

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

// 🧠 天才的戦略判定システム（b3.cppの知恵+機械学習）
struct GeniusStrategy {
    bool use_palette_complex;
    bool enable_aggressive_pruning;
    ll max_ca, max_cb, max_cc;
    ll color_selection_limit;
    ll color_loop_limit1, color_loop_limit2, color_loop_limit3;

    GeniusStrategy(ll k, ll h, ll t, ll d) {
        // 🚀 b3.cpp風戦略分岐
        bool is_high_performance_needed = (t > 16500 && d < 1200);
        bool is_large_case              = (h >= 500); // h=1000対策

        if (is_high_performance_needed && !is_large_case) {
            // 複雑戦略（パレット利用）
            use_palette_complex       = true;
            enable_aggressive_pruning = false;
            color_selection_limit     = min(k, 6LL);

            if (k <= 4) {
                max_ca = 14;
                max_cb = 14;
                max_cc = 14;
            } else if (k == 5) {
                max_ca = 8;
                max_cb = 8;
                max_cc = 7;
            } else {
                max_ca = 7;
                max_cb = 7;
                max_cc = 7;
            }

            color_loop_limit1 = color_selection_limit;
            color_loop_limit2 = color_selection_limit;
            color_loop_limit3 = color_selection_limit;

        } else {
            // 🏃‍♂️ 超高速戦略（大規模対応）
            use_palette_complex       = false;
            enable_aggressive_pruning = true;

            // h=1000対策：激的な削減
            if (is_large_case) {
                color_selection_limit = min(k, 4LL); // 色数を大幅削減
                max_ca                = 3;
                max_cb                = 3;
                max_cc                = 2; // ループ回数削減

                // コスト効率で更に調整
                if (d > 1000) {
                    max_ca                = 2;
                    max_cb                = 2;
                    max_cc                = 1;
                    color_selection_limit = min(k, 3LL);
                } else if (d <= 500) {
                    max_ca                = 4;
                    max_cb                = 3;
                    max_cc                = 3;
                    color_selection_limit = min(k, 5LL);
                }

                // 色ループも制限
                color_loop_limit1 = min(color_selection_limit, 2LL);
                color_loop_limit2 = color_selection_limit;
                color_loop_limit3 = color_selection_limit;

            } else {
                // 中規模：機械学習結果適用
                color_selection_limit = min(k, 8LL);
                max_ca                = 7;
                max_cb                = 5;
                max_cc                = 4; // 学習最適値

                color_loop_limit1 = color_selection_limit;
                color_loop_limit2 = color_selection_limit;
                color_loop_limit3 = color_selection_limit;

                // 動的調整
                if (t > 30000) {
                    max_ca                = 5;
                    max_cb                = 3;
                    max_cc                = 2;
                    color_selection_limit = min(k, 6LL);
                }
            }
        }
    }
};

// FPS色選別（b3.cpp準拠）
double color_dist2(const Paint &a, const Paint &b) {
    double dr = a.red - b.red;
    double dg = a.green - b.green;
    double db = a.blue - b.blue;
    return dr * dr + dg * dg + db * db;
}

vector<int> select_genius_colors(const vector<Paint> &colors, int target_count) {
    const int k = colors.size();
    vector<int> selected;

    if (k <= target_count) {
        selected.resize(k);
        iota(selected.begin(), selected.end(), 0);
        return selected;
    }

    // RGB軸最近色選択
    int r_best = 0, g_best = 0, b_best = 0;
    double r_dist = colors[0].green * colors[0].green + colors[0].blue * colors[0].blue;
    double g_dist = colors[0].red * colors[0].red + colors[0].blue * colors[0].blue;
    double b_dist = colors[0].red * colors[0].red + colors[0].green * colors[0].green;

    for (int i = 1; i < k; ++i) {
        double d_r = colors[i].green * colors[i].green + colors[i].blue * colors[i].blue;
        double d_g = colors[i].red * colors[i].red + colors[i].blue * colors[i].blue;
        double d_b = colors[i].red * colors[i].red + colors[i].green * colors[i].green;

        if (d_r < r_dist) {
            r_dist = d_r;
            r_best = i;
        }
        if (d_g < g_dist) {
            g_dist = d_g;
            g_best = i;
        }
        if (d_b < b_dist) {
            b_dist = d_b;
            b_best = i;
        }
    }

    selected.push_back(r_best);
    if (g_best != r_best) selected.push_back(g_best);
    if (b_best != r_best && b_best != g_best) selected.push_back(b_best);

    if (selected.size() >= target_count) {
        selected.resize(target_count);
        return selected;
    }

    // FPS補完
    vector<double> min_dist(k, 1e30);
    for (int idx : selected) {
        for (int i = 0; i < k; ++i) {
            min_dist[i] = min(min_dist[i], color_dist2(colors[i], colors[idx]));
        }
    }

    while (selected.size() < target_count) {
        int best_i       = -1;
        double best_dist = -1;
        for (int i = 0; i < k; ++i) {
            if (min_dist[i] > best_dist) {
                best_dist = min_dist[i];
                best_i    = i;
            }
        }
        if (best_i == -1) break;

        selected.push_back(best_i);
        for (int i = 0; i < k; ++i) {
            min_dist[i] = min(min_dist[i], color_dist2(colors[i], colors[best_i]));
        }
    }

    return selected;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k, Paint());
    vector<Paint> target_color(h, Paint());

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    // 🧠 天才戦略決定
    GeniusStrategy strategy(k, h, t, d);

    // 🎨 色選別
    vector<int> selected = select_genius_colors(own_color, strategy.color_selection_limit);
    vector<Paint> colors(selected.size());
    for (int i = 0; i < selected.size(); ++i) { colors[i] = own_color[selected[i]]; }
    int actual_k = colors.size();

    // パレット初期化
    if (strategy.use_palette_complex) {
        // b3.cpp風複雑パレット
        rep(ii, n) {
            rep(i, n - 1) {
                if ((i == 0 && ii > 1) || (ii == 1 && i == 1)) {
                    cout << 1 << " ";
                } else {
                    cout << 0 << " ";
                }
            }
            cout << el;
        }
        rep(ii, n - 1) {
            rep(i, n) {
                if ((ii == 0 && i > 1) || (ii == 1 && i == 1)) {
                    cout << 1 << " ";
                } else {
                    cout << 0 << " ";
                }
            }
            cout << el;
        }
    } else {
        // シンプルパレット
        rep(ii, n) {
            rep(i, n - 1) cout << 0 << " ";
            cout << el;
        }
        rep(ii, n - 1) {
            rep(i, n) cout << 0 << " ";
            cout << el;
        }
    }

    ll c = (t / h) / 2;
    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &target = target_color[hi];
        double error  = 101010;

        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1, dis_count = 0;
        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;

        // 🚀 超効率探索（積極的枝刈り）
        rep(c1, strategy.color_loop_limit1) {
            ll c2_start = strategy.use_palette_complex ? 0 : c1 + 1;
            reps(c2, c2_start, min((ll)actual_k, c2_start + strategy.color_loop_limit2)) {
                ll c3_start = strategy.use_palette_complex ? 0 : c2 + 1;
                reps(c3, c3_start, min((ll)actual_k, c3_start + strategy.color_loop_limit3)) {

                    if (c1 >= actual_k || c2 >= actual_k || c3 >= actual_k) continue;

                    Paint &color1 = colors[c1];
                    Paint &color2 = colors[c2];
                    Paint &color3 = colors[c3];

                    ll use_now_color = -qq;
                    rep(dis, 4) {
                        use_now_color += (dis < rr ? qq + 1 : qq);

                        rep(ca, strategy.max_ca + 1) {
                            rep(cb, strategy.max_cb + 1) {
                                rep(cc, strategy.max_cc + 1) {
                                    // 🚀 超積極的枝刈り
                                    if (strategy.enable_aggressive_pruning) {
                                        if (ca + cb + cc == 0) continue;
                                        ll penalty = (ca > 0) + (cb > 0) + (cc > 0);
                                        if (penalty * d >= error) continue; // 早期カット
                                    }

                                    if (ca + cb + cc > c) break;
                                    if (ca + cb + cc + use_now_color < 1) continue;

                                    ll sum = ca + cb + cc + use_now_color;

                                    // 高速化計算
                                    double inv_sum = 1.0 / sum;
                                    double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                                now_color.red * use_now_color) *
                                               inv_sum;
                                    double g = (color1.green * ca + color2.green * cb +
                                                color3.green * cc + now_color.green * use_now_color) *
                                               inv_sum;
                                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                                now_color.blue * use_now_color) *
                                               inv_sum;

                                    double dr = r - target.red;
                                    double dg = g - target.green;
                                    double db = b - target.blue;
                                    double n_error =
                                        sqrt(dr * dr + dg * dg + db * db) * 10000 + (ca + cb + cc) * d;

                                    if (chmin(error, n_error)) {
                                        usecount_a      = ca;
                                        usecount_b      = cb;
                                        usecount_c      = cc;
                                        use_id1         = selected[c1];
                                        use_id2         = selected[c2];
                                        use_id3         = selected[c3];
                                        new_color.red   = r;
                                        new_color.green = g;
                                        new_color.blue  = b;
                                        new_color.count = sum;
                                        dis_count       = now_color.count - use_now_color;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        now_color = new_color;

        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
}