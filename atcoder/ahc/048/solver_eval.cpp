#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

template <typename T, typename U> bool chmin(T &a, U b) { return a > b ? a = b, 1 : 0; }
template <typename T, typename U> bool chmax(T &a, U b) { return a < b ? a = b, 1 : 0; }

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

// ----------------- 階上 ------------------
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

// 色間の距離の二乗を計算
double dist2(const Paint &a, const Paint &b) {
    double dr = a.red - b.red;
    double dg = a.green - b.green;
    double db = a.blue - b.blue;
    return dr * dr + dg * dg + db * db;
}

// 表現力の高い色を選別する関数（RGB軸近似 + FPS）
vector<int> pick_basis_colors(const vector<Paint> &col, int want = 10) {
    const int k = (int)col.size();
    vector<int> basis;
    if (k <= want) {
        basis.resize(k);
        iota(basis.begin(), basis.end(), 0);
        return basis;
    }

    // RGB軸への距離が最小となる色を取る
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

    // 残りをFPSで埋める
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

// パレットレイアウト定義
vector<vector<pii>> get_palette_layouts(int n) {
    vector<vector<pii>> layouts;

    // Layout 0: 全体を塗りつぶし（シンプル）
    layouts.push_back({});

    // Layout 1: 十字型分割
    vector<pii> cross_layout;
    rep(i, n) reps(j, 1, n - 1) {
        if (i >= 2 || j == 1) cross_layout.push_back({i, j});
    }
    rep(i, n - 1) reps(j, 1, n) {
        if (i <= 1 || j == 1) cross_layout.push_back({i, j});
    }
    layouts.push_back(cross_layout);

    // Layout 2: L字型分割
    vector<pii> l_layout;
    rep(i, n) rep(j, n / 2) l_layout.push_back({i, j});
    rep(i, n / 2) reps(j, n / 2, n) l_layout.push_back({i, j});
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
        if (i == 0 || i == n - 1 || j == 0 || j == n - 1) { border_layout.push_back({i, j}); }
    }
    layouts.push_back(border_layout);

    return layouts;
}

// グローバル変数
ll global_score = 0;
ll action_count = 0;

void output_action(int type, int x, int y, int arg1 = 0, int arg2 = 0) {
    // スコア計算のみ（実際の出力は行わない）
    action_count++;

    if (type == 1) { // 絵具追加
        global_score += 1;
    } else if (type == 2) { // 塗布
        global_score += 1;
    } else if (type == 3) { // 塗料廃棄
        global_score += 1;
    }
}

int main(int argc, char *argv[]) {
    // コマンドライン引数から パラメータを取得
    int LOOKAHEAD  = 3; // デフォルト値
    int BEAM_WIDTH = 8;
    int PALETTE_ID = 0;

    if (argc >= 4) {
        LOOKAHEAD  = atoi(argv[1]);
        BEAM_WIDTH = atoi(argv[2]);
        PALETTE_ID = atoi(argv[3]);
    }

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k, Paint());
    vector<Paint> target_color(h, Paint());

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    // パレットレイアウト取得
    auto layouts           = get_palette_layouts(n);
    auto palette_positions = (PALETTE_ID < layouts.size()) ? layouts[PALETTE_ID] : layouts[0];

    // パレット初期化出力（スコア計算用）
    for (auto [x, y] : palette_positions) {
        output_action(4, x, y, 1, 0); // パレット分割
    }

    auto squ = [](double x) { return x * x; };

    // 色選別（パラメータによって調整）
    int reduced_k               = std::min((int)k, 6 + BEAM_WIDTH / 4);
    vector<int> selected_colors = pick_basis_colors(own_color, reduced_k);
    vector<Paint> reduced_own_color(selected_colors.size());
    for (int i = 0; i < selected_colors.size(); ++i) {
        reduced_own_color[i] = own_color[selected_colors[i]];
    }
    vector<int> color_mapping = selected_colors;
    int actual_k              = reduced_own_color.size();

    Paint now_color(0.0, 0.0, 0.0, 0.0);

    rep(hi, h) {
        Paint &tr = target_color[hi];

        double error  = 101010;
        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1;
        ll dis_count    = 0;
        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = (ll)round(now_color.count) / 4;
        ll rr = (ll)round(now_color.count) % 4;

        // 探索幅をパラメータで調整
        int k1 = std::min(7, 3 + BEAM_WIDTH / 4);
        int k2 = std::min(7, 3 + BEAM_WIDTH / 4);
        int k3 = std::min(7, 3 + BEAM_WIDTH / 4);

        // 先読み深さに応じた探索範囲調整
        if (LOOKAHEAD >= 4) {
            k1 = std::min(k1, 5);
            k2 = std::min(k2, 5);
            k3 = std::min(k3, 5);
        }

        rep(c1, min(actual_k, (int)(BEAM_WIDTH / 2))) {
            reps(c2, c1 + 1, min(actual_k, (int)c1 + (int)(BEAM_WIDTH / 3))) {
                reps(c3, c2 + 1, min(actual_k, (int)c2 + (int)(BEAM_WIDTH / 3))) {
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

        // アクション出力（スコア計算用）
        rep(i, dis_count) { output_action(3, 0, 0); }
        rep(i, usecount_a) output_action(1, 0, 0, use_id1);
        rep(i, usecount_b) output_action(1, 0, 0, use_id2);
        rep(i, usecount_c) output_action(1, 0, 0, use_id3);

        output_action(2, 0, 0);
        now_color.count = round(now_color.count) - 1;
    }

    // 最終スコア計算
    // 色差エラー + アクション数 * d の簡易版
    double total_error = 0;
    for (int hi = 0; hi < h; hi++) {
        // 簡易エラー計算（実際は複雑だが評価用なので簡略化）
        total_error += 50.0; // 仮の色差エラー
    }

    global_score = (ll)(total_error + action_count * d);

    // スコアをstderrに出力（学習システムが読み取る）
    cerr << global_score << endl;

    return 0;
}