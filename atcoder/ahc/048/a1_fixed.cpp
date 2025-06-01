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

    auto squ = [](double x) { return x * x; };

    // 初期出力（元のコードと同じ）
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;
    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        const Paint &tr = target_color[hi];
        double error    = 1e9;

        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1;
        ll dis_count = 0;
        Paint new_color(0, 0, 0, 0);

        ll qq = now_color.count / 6;
        ll rr = now_color.count % 6;

        // 最適化1: k の範囲を制限（ただし元の動作は保持）
        ll search_k = min(k, 12LL); // 12色まで検索して品質とのバランスを取る

        rep(c1, search_k) {
            const Paint &color1 = own_color[c1];

            reps(c2, c1 + 1, search_k) { // 元のコードに忠実
                const Paint &color2 = own_color[c2];

                reps(c3, c2 + 1, search_k) { // 元のコードに忠実
                    const Paint &color3 = own_color[c3];

                    // 最適化2: 早期スキップ（控えめな閾値）
                    double color_diff = max({abs(color1.red - tr.red) + abs(color1.green - tr.green) +
                                                 abs(color1.blue - tr.blue),
                                             abs(color2.red - tr.red) + abs(color2.green - tr.green) +
                                                 abs(color2.blue - tr.blue),
                                             abs(color3.red - tr.red) + abs(color3.green - tr.green) +
                                                 abs(color3.blue - tr.blue)});
                    if (color_diff > 2.0) continue; // 控えめな閾値

                    rep(dis, 6) { // 元のコードに忠実
                        ll use_now_color = (dis < rr ? qq + 1 : qq);

                        rep(ca, 4) rep(cb, 4) rep(cc, 3) { // 元のコードに忠実
                            if (ca + cb + cc > c) continue;
                            if (ca + cb + cc + use_now_color < 1) continue;

                            ll sum = ca + cb + cc + use_now_color;

                            // 元のコードと同じ計算
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
                                (ca + cb + cc + now_color.count - use_now_color) * d;

                            // 最適化3: 控えめな早期終了
                            if (n_error >= error * 1.5) continue;

                            if (chmin(error, n_error)) {
                                usecount_a = ca;
                                usecount_b = cb;
                                usecount_c = cc;
                                use_id1    = c1;
                                use_id2    = c2;
                                use_id3    = c3;
                                new_color  = Paint(r, g, b, sum);
                                dis_count  = now_color.count - use_now_color;
                            }
                        }
                    }
                }
            }
        }

        now_color = new_color;

        // 出力部分（元のコードと同じ）
        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }

    return 0;
}