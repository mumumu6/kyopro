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

#include "param_model.h" // 機械学習モデル

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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(10);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k, Paint());
    vector<Paint> target_color(h, Paint());

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    // 🤖 機械学習によるパラメータ最適化
    vector<array<double, 3>> own_colors_ml(k), target_colors_ml(h);
    for (int i = 0; i < k; i++) {
        own_colors_ml[i] = {own_color[i].red, own_color[i].green, own_color[i].blue};
    }
    for (int i = 0; i < h; i++) {
        target_colors_ml[i] = {target_color[i].red, target_color[i].green, target_color[i].blue};
    }

    auto features = ml_params::extract_features(k, h, t, d, own_colors_ml, target_colors_ml);
    auto [LOOKAHEAD, BEAM_WIDTH, PALETTE_ID] = ml_params::predict_all_params(features);

    // デバッグ出力
    cerr << "🤖 ML最適化: L=" << LOOKAHEAD << ", B=" << BEAM_WIDTH << ", P=" << PALETTE_ID << endl;
    cerr << "特徴量: K=" << k << ", H=" << h << ", T=" << t << ", D=" << d << endl;

    // パレット初期化出力
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << endl;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << endl;
    }

    auto squ = [](double x) { return x * x; };

    // 色選別（機械学習パラメータによって調整）
    int reduced_k               = min((int)k, 6 + BEAM_WIDTH / 4);
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

        // 探索幅を機械学習パラメータで調整
        int k1 = min(7, 3 + BEAM_WIDTH / 4);
        int k2 = min(7, 3 + BEAM_WIDTH / 4);
        int k3 = min(7, 3 + BEAM_WIDTH / 4);

        // 先読み深さに応じた探索範囲調整
        if (LOOKAHEAD >= 4) {
            k1 = min(k1, 5);
            k2 = min(k2, 5);
            k3 = min(k3, 5);
        }

        // 先読みロジック（簡易版）
        int lookahead_targets = min(LOOKAHEAD, (int)(h - hi));
        vector<Paint> upcoming_targets;
        for (int future = 0; future < lookahead_targets && hi + future < h; future++) {
            upcoming_targets.push_back(target_color[hi + future]);
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

                            // 基本エラー計算
                            double n_error =
                                sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                                (ca + cb + cc) * d;

                            // 先読み補正（簡易版）
                            if (LOOKAHEAD > 1 && upcoming_targets.size() > 1) {
                                double future_penalty = 0;
                                for (int fi = 1; fi < upcoming_targets.size(); fi++) {
                                    auto &future_target = upcoming_targets[fi];
                                    double future_error =
                                        sqrt(squ(r - future_target.red) + squ(g - future_target.green) +
                                             squ(b - future_target.blue));
                                    future_penalty +=
                                        future_error * (LOOKAHEAD - fi) * 100; // 重み付き先読み
                                }
                                n_error += future_penalty / lookahead_targets;
                            }

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
        rep(i, dis_count) { cout << 3 << " " << 0 << " " << 0 << endl; }
        rep(i, usecount_a) cout << 1 << " " << 0 << " " << 0 << " " << use_id1 << endl;
        rep(i, usecount_b) cout << 1 << " " << 0 << " " << 0 << " " << use_id2 << endl;
        rep(i, usecount_c) cout << 1 << " " << 0 << " " << 0 << " " << use_id3 << endl;

        cout << 2 << " " << 0 << " " << 0 << endl;
        now_color.count = round(now_color.count) - 1;
    }

    return 0;
}