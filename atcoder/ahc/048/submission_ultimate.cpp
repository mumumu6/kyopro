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
};

// 🤖 20000ケース機械学習結果 + 高度戦略判定
struct UltimateMLStrategy {
    bool use_complex_strategy;
    ll max_ca, max_cb, max_cc;
    ll color_limit;
    bool use_palette_optimization;

    UltimateMLStrategy(ll k, ll h, ll t, ll d) {
        // 機械学習結果による戦略分岐判定
        // 学習結果: t>16500 && d<1200 で複雑戦略が有効
        use_complex_strategy = (t > 15000 && d < 1500) || (k >= 15 && t > 20000);

        if (use_complex_strategy) {
            // 🚀 複雑戦略：パレット+高精度色混合
            use_palette_optimization = true;
            color_limit              = min(k, 8LL); // 色選別を厳格に

            // 学習結果を反映したパラメータ
            if (k <= 6) {
                max_ca = 12;
                max_cb = 10;
                max_cc = 8; // 少ない色は多用
            } else if (k <= 10) {
                max_ca = 8;
                max_cb = 6;
                max_cc = 4; // 中程度
            } else {
                max_ca = 6;
                max_cb = 4;
                max_cc = 3; // 多い色は制限
            }

            // dによる調整
            if (d > 800) {
                max_ca = max(1LL, max_ca - 2);
                max_cb = max(1LL, max_cb - 2);
                max_cc = max(1LL, max_cc - 1);
            }

        } else {
            // 🏃 高速戦略：シンプルだが効率的
            use_palette_optimization = false;
            color_limit              = min(k, 12LL);

            // 機械学習最適化パラメータ
            max_ca = 7;
            max_cb = 5;
            max_cc = 4; // 学習結果：CC=4が最適

            // 動的調整（学習データより）
            if (t > 40000) {
                max_ca      = 5;
                max_cb      = 3;
                max_cc      = 2; // 大規模は制限
                color_limit = min(k, 8LL);
            } else if (t < 8000) {
                max_ca      = 9;
                max_cb      = 7;
                max_cc      = 5; // 小規模は拡張
                color_limit = min(k, 15LL);
            }

            // コスト効率（学習で重要と判明）
            double cost_ratio = (double)d / t;
            if (cost_ratio > 0.4) {
                max_ca = max(1LL, max_ca - 1);
                max_cb = max(1LL, max_cb - 1);
                max_cc = max(1LL, max_cc - 1);
            }
        }
    }
};

// 色間距離計算
double color_dist2(const Paint &a, const Paint &b) {
    double dr = a.red - b.red;
    double dg = a.green - b.green;
    double db = a.blue - b.blue;
    return dr * dr + dg * dg + db * db;
}

// 🎨 高度色選別アルゴリズム（RGB軸近似+FPS）
vector<int> select_optimal_colors(const vector<Paint> &colors, int target_count) {
    const int k = colors.size();
    vector<int> selected;

    if (k <= target_count) {
        selected.resize(k);
        iota(selected.begin(), selected.end(), 0);
        return selected;
    }

    // RGB軸に最も近い色を選択
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

    // FPS（最遠点サンプリング）で残りを選択
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

    // 🤖 機械学習戦略判定
    UltimateMLStrategy strategy(k, h, t, d);

    // 🎨 最適色選別
    vector<int> selected_colors = select_optimal_colors(own_color, strategy.color_limit);
    vector<Paint> reduced_colors(selected_colors.size());
    for (int i = 0; i < selected_colors.size(); ++i) {
        reduced_colors[i] = own_color[selected_colors[i]];
    }
    int actual_k = reduced_colors.size();

    auto squ = [](double x) { return x * x; };

    // 🎨 戦略的パレット初期化
    if (strategy.use_palette_optimization) {
        // 複雑戦略：パレット活用
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
        // シンプル戦略：標準初期化
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

        // 🚀 動的範囲調整（機械学習結果）
        ll range_c1 = min((ll)actual_k, strategy.color_limit);
        ll range_c2 = strategy.use_complex_strategy ? actual_k : min((ll)actual_k, range_c1 + 4);
        ll range_c3 = strategy.use_complex_strategy ? actual_k : min((ll)actual_k, range_c2 + 2);

        rep(c1, range_c1) {
            ll c2_start = strategy.use_complex_strategy ? 0 : c1 + 1;
            ll c2_end   = min((ll)actual_k, c2_start + (range_c2 - c2_start));

            reps(c2, c2_start, c2_end) {
                ll c3_start = strategy.use_complex_strategy ? 0 : c2 + 1;
                ll c3_end   = min((ll)actual_k, c3_start + (range_c3 - c3_start));

                reps(c3, c3_start, c3_end) {
                    Paint &color1 = reduced_colors[c1];
                    Paint &color2 = reduced_colors[c2];
                    Paint &color3 = reduced_colors[c3];

                    ll use_now_color = -qq;
                    rep(dis, 4) {
                        use_now_color += (dis < rr ? qq + 1 : qq);

                        // 🤖 機械学習最適化パラメータ適用
                        rep(ca, strategy.max_ca) {
                            rep(cb, strategy.max_cb) {
                                rep(cc, strategy.max_cc) {
                                    if (ca + cb + cc > c) break; // 早期終了
                                    if (ca + cb + cc + use_now_color < 1) continue;

                                    ll sum = ca + cb + cc + use_now_color;

                                    // 高速化：逆数事前計算
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

                                    // 高速化：関数呼び出し削減
                                    double dr = r - target.red;
                                    double dg = g - target.green;
                                    double db = b - target.blue;
                                    double n_error =
                                        sqrt(dr * dr + dg * dg + db * db) * 10000 + (ca + cb + cc) * d;

                                    if (chmin(error, n_error)) {
                                        usecount_a      = ca;
                                        usecount_b      = cb;
                                        usecount_c      = cc;
                                        use_id1         = selected_colors[c1];
                                        use_id2         = selected_colors[c2];
                                        use_id3         = selected_colors[c3];
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

        // アクション出力
        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
}