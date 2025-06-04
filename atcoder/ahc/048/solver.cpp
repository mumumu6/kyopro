#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
#include "param_model.h" // 機械学習による自動パラメータ予測

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

// ---------- 既存のコード（b3.cppより） ----------
mint fact(ll n, bool inv = false) {
    if (n < 0) return 0;
    static vector<mint> factorial     = {1};
    static vector<mint> factorial_inv = {1};
    ll siz                            = factorial.size();
    if (n >= siz) {
        reps(i, siz, n + 1) factorial.emplace_back(factorial[i - 1] * i);
        factorial_inv.resize(n + 1);
        factorial_inv[n] = factorial[n].inv();
        for (ll i = n; i > siz; i--) factorial_inv[i - 1] = factorial_inv[i] * i;
    }
    return inv ? factorial_inv[n] : factorial[n];
}

mint perm(ll n, ll k) { return fact(n) * fact(n - k, true); }
mint comb(ll n, ll k) { return perm(n, k) * fact(k, true); }

struct Paint {
    double red, green, blue;
    double count = 1;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0)
        : red(r), green(g), blue(b), count(c) {}
};

double dist2(const Paint &a, const Paint &b) {
    double dr = a.red - b.red;
    double dg = a.green - b.green;
    double db = a.blue - b.blue;
    return dr * dr + dg * dg + db * db;
}

vector<int> pick_basis_colors(const vector<Paint> &col, int want = 10) {
    const int k = (int)col.size();
    vector<int> basis;
    if (k <= want) {
        basis.resize(k);
        iota(basis.begin(), basis.end(), 0);
        return basis;
    }

    int r_idx = 0, g_idx = 0, b_idx = 0;
    double best_r_dist2 = col[0].green * col[0].green + col[0].blue * col[0].blue;
    double best_g_dist2 = col[0].red * col[0].red + col[0].blue * col[0].blue;
    double best_b_dist2 = col[0].red * col[0].red + col[0].green * col[0].green;

    for (int i = 1; i < k; ++i) {
        double d_r = col[i].green * col[i].green + col[i].blue * col[i].blue;
        if (d_r < best_r_dist2) {
            best_r_dist2 = d_r;
            r_idx        = i;
        }
        double d_g = col[i].red * col[i].red + col[i].blue * col[i].blue;
        if (d_g < best_g_dist2) {
            best_g_dist2 = d_g;
            g_idx        = i;
        }
        double d_b = col[i].red * col[i].red + col[i].green * col[i].green;
        if (d_b < best_b_dist2) {
            best_b_dist2 = d_b;
            b_idx        = i;
        }
    }

    basis.push_back(r_idx);
    if (g_idx != r_idx) basis.push_back(g_idx);
    if (b_idx != r_idx && b_idx != g_idx) basis.push_back(b_idx);

    if ((int)basis.size() >= want) {
        basis.resize(want);
        return basis;
    }

    vector<double> mind(k, 1e30);
    for (int idx : basis) {
        for (int i = 0; i < k; ++i) { mind[i] = min(mind[i], dist2(col[i], col[idx])); }
    }
    while ((int)basis.size() < want) {
        int best_i   = -1;
        double bestd = -1;
        for (int i = 0; i < k; ++i) {
            if (mind[i] > bestd) {
                bestd  = mind[i];
                best_i = i;
            }
        }
        if (best_i == -1) break;
        basis.push_back(best_i);
        for (int i = 0; i < k; ++i) { mind[i] = min(mind[i], dist2(col[i], col[best_i])); }
    }

    return basis;
}

Paint calculate_weighted_color_average(const vector<Paint> &colors, const vector<double> &weights,
                                       double base_amount = 0.0, const Paint &base_color = Paint()) {
    if (colors.size() != weights.size()) { return Paint(); }

    double total_weight = base_amount;
    double red_sum      = base_color.red * base_amount;
    double green_sum    = base_color.green * base_amount;
    double blue_sum     = base_color.blue * base_amount;

    for (int i = 0; i < colors.size(); i++) {
        if (weights[i] >= 0) {
            total_weight += weights[i];
            red_sum += colors[i].red * weights[i];
            green_sum += colors[i].green * weights[i];
            blue_sum += colors[i].blue * weights[i];
        }
    }

    if (total_weight == 0) { return Paint(0.0, 0.0, 0.0, 0.0); }

    return Paint(red_sum / total_weight, green_sum / total_weight, blue_sum / total_weight,
                 total_weight);
}

// ---------- パレットレイアウト定義 ----------
vector<vector<pii>> get_palette_layouts(int n) {
    vector<vector<pii>> layouts;

    // Layout 0: 全体塗りつぶし（シンプル）
    layouts.push_back({});

    // Layout 1: 十字型分割
    vector<pii> cross_layout;
    rep(i, n) reps(j, 1, n - 1) {
        if ((i >= 2 && i <= n - 3) || j == 1) cross_layout.push_back({i, j});
    }
    rep(i, n - 1) reps(j, 1, n) {
        if ((i <= 1 || i >= n - 2) || j == 1) cross_layout.push_back({i, j});
    }
    layouts.push_back(cross_layout);

    // Layout 2: L字型分割
    vector<pii> l_layout;
    rep(i, n) rep(j, n / 2 + 1) l_layout.push_back({i, j});
    rep(i, n / 2 + 1) reps(j, n / 2 + 1, n) l_layout.push_back({i, j});
    layouts.push_back(l_layout);

    // Layout 3: 格子分割
    vector<pii> grid_layout;
    rep(i, n) rep(j, n) {
        if ((i + j) % 2 == 0) grid_layout.push_back({i, j});
    }
    layouts.push_back(grid_layout);

    // Layout 4: 周辺分割
    vector<pii> border_layout;
    rep(i, n) rep(j, n) {
        if (i <= 1 || i >= n - 2 || j <= 1 || j >= n - 2) { border_layout.push_back({i, j}); }
    }
    layouts.push_back(border_layout);

    return layouts;
}

// ---------- 先読み対応ビームサーチ ----------
struct BeamState {
    Paint current_color;
    double cumulative_error;
    vector<vector<int>> action_history; // [target_id][action_type, params...]

    BeamState(Paint color = Paint(), double error = 0.0)
        : current_color(color), cumulative_error(error) {}
};

vector<BeamState> beam_search_lookahead(const vector<Paint> &target_colors,
                                        const vector<Paint> &own_colors,
                                        const vector<int> &color_mapping, int start_target_id,
                                        int lookahead_depth, int beam_width, ll d, ll t, ll h) {
    vector<BeamState> beam = {BeamState()};

    for (int depth = 0; depth < min(lookahead_depth, (int)target_colors.size() - start_target_id);
         depth++) {
        int target_id = start_target_id + depth;
        Paint &target = const_cast<Paint &>(target_colors[target_id]);

        vector<BeamState> next_beam;

        for (const auto &state : beam) {
            // 現在の状態から次の行動を探索
            Paint now_color   = state.current_color;
            double best_error = 1e9;
            vector<int> best_action;
            Paint best_new_color;

            ll qq = (ll)round(now_color.count) / 4;
            ll rr = (ll)round(now_color.count) % 4;

            // 簡略化された探索（計算時間削減）
            rep(c1, min((int)own_colors.size(), beam_width / 2)) {
                reps(c2, c1 + 1, min((int)own_colors.size(), (int)c1 + (int)beam_width / 3)) {
                    reps(c3, c2 + 1, min((int)own_colors.size(), (int)c2 + (int)beam_width / 3)) {
                        const Paint &color1 = own_colors[c1];
                        const Paint &color2 = own_colors[c2];
                        const Paint &color3 = own_colors[c3];

                        ll use_now_color = -qq + (0 < rr ? qq + 1 : qq);

                        rep(ca, 4) rep(cb, 4) rep(cc, 4) {
                            ll c_limit = (t / h) / 3;
                            if (ca + cb + cc > c_limit) continue;
                            if (ca + cb + cc + use_now_color < 1) continue;

                            ll sum = ca + cb + cc + use_now_color;

                            double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                        now_color.red * use_now_color) /
                                       sum;
                            double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                        now_color.green * use_now_color) /
                                       sum;
                            double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                        now_color.blue * use_now_color) /
                                       sum;

                            double error = sqrt((r - target.red) * (r - target.red) +
                                                (g - target.green) * (g - target.green) +
                                                (b - target.blue) * (b - target.blue)) *
                                               10000 +
                                           (ca + cb + cc) * d;

                            if (error < best_error) {
                                best_error     = error;
                                best_action    = {(int)ca,
                                                  (int)cb,
                                                  (int)cc,
                                                  color_mapping[c1],
                                                  color_mapping[c2],
                                                  color_mapping[c3],
                                                  (int)(now_color.count - use_now_color)};
                                best_new_color = Paint(r, g, b, sum - 1);
                            }
                        }
                    }
                }
            }

            BeamState new_state(best_new_color, state.cumulative_error + best_error);
            new_state.action_history = state.action_history;
            new_state.action_history.push_back(best_action);
            next_beam.push_back(new_state);
        }

        // ビーム選択（上位beam_width個を保持）
        sort(next_beam.begin(), next_beam.end(), [](const BeamState &a, const BeamState &b) {
            return a.cumulative_error < b.cumulative_error;
        });

        if (next_beam.size() > beam_width) { next_beam.resize(beam_width); }

        beam = next_beam;
    }

    return beam;
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

    // ---------- 機械学習による自動パラメータ予測 ----------
    vector<array<double, 3>> own_ml(k), target_ml(h);
    rep(i, k) own_ml[i]    = {own_color[i].red, own_color[i].green, own_color[i].blue};
    rep(i, h) target_ml[i] = {target_color[i].red, target_color[i].green, target_color[i].blue};

    auto features = ml_params::extract_features(k, h, t, d, own_ml, target_ml);
    auto [LOOKAHEAD, BEAM_WIDTH, PALETTE_ID] = ml_params::predict_all_params(features);

    // デバッグ出力（提出時は削除）
    // cerr << "ML Prediction: L=" << LOOKAHEAD << ", B=" << BEAM_WIDTH << ", P=" << PALETTE_ID << endl;

    // ---------- パレット初期化 ----------
    auto layouts           = get_palette_layouts(n);
    auto palette_positions = (PALETTE_ID < layouts.size()) ? layouts[PALETTE_ID] : layouts[0];

    // パレット分割出力
    rep(ii, n) {
        rep(i, n - 1) {
            bool is_palette = false;
            for (auto [px, py] : palette_positions) {
                if (ii == px && i == py) {
                    is_palette = true;
                    break;
                }
            }
            cout << (is_palette ? 1 : 0) << " ";
        }
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) {
            bool is_palette = false;
            for (auto [px, py] : palette_positions) {
                if (ii == px && i == py) {
                    is_palette = true;
                    break;
                }
            }
            cout << (is_palette ? 1 : 0) << " ";
        }
        cout << el;
    }

    auto squ = [](double x) { return x * x; };

    // ---------- 色選別（パラメータ適応） ----------
    int reduced_k = min((int)k, 6 + BEAM_WIDTH / 4);
    if (t > 20000 && d < 800) {
        reduced_k = min((int)k, 8); // 複雑分岐用
    }

    vector<int> selected_colors = pick_basis_colors(own_color, reduced_k);
    vector<Paint> reduced_own_color(selected_colors.size());
    for (int i = 0; i < selected_colors.size(); ++i) {
        reduced_own_color[i] = own_color[selected_colors[i]];
    }
    vector<int> color_mapping = selected_colors;
    int actual_k              = reduced_own_color.size();

    Paint now_color(0.0, 0.0, 0.0, 0.0);

    // ---------- メインループ（先読み対応） ----------
    rep(hi, h) {
        Paint &tr = target_color[hi];

        // 先読みビームサーチまたは通常探索
        if (LOOKAHEAD > 1 && hi + LOOKAHEAD <= h) {
            auto beam_results = beam_search_lookahead(target_color, reduced_own_color, color_mapping, hi,
                                                      LOOKAHEAD, BEAM_WIDTH, d, t, h);

            if (!beam_results.empty()) {
                // 最良の行動を採用
                auto best_action = beam_results[0].action_history[0];

                ll dis_count  = best_action[6];
                ll usecount_a = best_action[0];
                ll usecount_b = best_action[1];
                ll usecount_c = best_action[2];
                ll use_id1    = best_action[3];
                ll use_id2    = best_action[4];
                ll use_id3    = best_action[5];

                // アクション実行
                rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
                rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
                rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
                rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;
                cout << 2 << spa << 0 << spa << 0 << el;

                now_color = beam_results[0].current_color;
                continue;
            }
        }

        // 通常探索（従来のロジック）
        double error  = 101010;
        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1;
        ll dis_count    = 0;
        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = (ll)round(now_color.count) / 4;
        ll rr = (ll)round(now_color.count) % 4;

        // 探索幅をパラメータで調整
        int k1 = min(7, 3 + BEAM_WIDTH / 4);
        int k2 = min(7, 3 + BEAM_WIDTH / 4);
        int k3 = min(7, 3 + BEAM_WIDTH / 4);

        if (d > 1500) { k1 = k2 = k3 = 3; }

        rep(c1, min(actual_k, BEAM_WIDTH / 2)) {
            reps(c2, c1 + 1, min(actual_k, (int)c1 + BEAM_WIDTH / 3)) {
                reps(c3, c2 + 1, min(actual_k, (int)c2 + BEAM_WIDTH / 3)) {
                    Paint &color1 = reduced_own_color[c1];
                    Paint &color2 = reduced_own_color[c2];
                    Paint &color3 = reduced_own_color[c3];

                    ll use_now_color = -qq;
                    rep(dis, 4) {
                        use_now_color += (dis < rr ? qq + 1 : qq);

                        rep(ca, k1) rep(cb, k2) rep(cc, k3) {
                            ll c_limit = (t / h) / 2;
                            if (ca + cb + cc > c_limit) continue;
                            if (ca + cb + cc + use_now_color < 1) continue;

                            ll sum = ca + cb + cc + use_now_color;

                            double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                        now_color.red * use_now_color) /
                                       sum;
                            double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                        now_color.green * use_now_color) /
                                       sum;
                            double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                        now_color.blue * use_now_color) /
                                       sum;

                            double n_error =
                                sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                                (ca + cb + cc) * d;

                            if (chmin(error, n_error)) {
                                usecount_a      = ca;
                                usecount_b      = cb;
                                usecount_c      = cc;
                                use_id1         = color_mapping[c1];
                                use_id2         = color_mapping[c2];
                                use_id3         = color_mapping[c3];
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

        now_color = new_color;

        // アクション出力
        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count = round(now_color.count) - 1;
    }

    return 0;
}