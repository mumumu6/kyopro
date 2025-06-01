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
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define spa " "
#define el '\n'

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}

    // 演算子オーバーロードで計算効率化
    Paint operator*(ll mult) const { return Paint(red * mult, green * mult, blue * mult, count); }

    Paint operator+(const Paint &other) const {
        return Paint(red + other.red, green + other.green, blue + other.blue, count + other.count);
    }
};

// 色差計算を事前計算してキャッシュ
struct ColorCalc {
    double red_diff_sq, green_diff_sq, blue_diff_sq;

    ColorCalc(const Paint &target)
        : red_diff_sq(target.red * target.red), green_diff_sq(target.green * target.green),
          blue_diff_sq(target.blue * target.blue) {}

    // sqrt計算を遅延して高速化
    double calc_error_sq(double r, double g, double b) const {
        double dr = r - red_diff_sq, dg = g - green_diff_sq, db = b - blue_diff_sq;
        return dr * dr + dg * dg + db * db;
    }
};

// 組み合わせ候補を事前生成して高速化
struct ColorCombination {
    ll c1, c2, c3, ca, cb, cc, use_now;
    double total_cost;

    ColorCombination(ll _c1, ll _c2, ll _c3, ll _ca, ll _cb, ll _cc, ll _use_now, double _cost)
        : c1(_c1), c2(_c2), c3(_c3), ca(_ca), cb(_cb), cc(_cc), use_now(_use_now), total_cost(_cost) {}
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

    // 初期出力は変更なし
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

    // 可能な組み合わせを事前生成（最大の最適化ポイント！）
    vector<ColorCombination> combinations;
    combinations.reserve(k * k * k * 6 * 4 * 4 * 3); // メモリ事前確保

    rep(hi, h) {
        const Paint &tr = target_color[hi];
        ColorCalc calc(tr);

        double best_error  = 1e9;
        ll best_usecount_a = 0, best_usecount_b = 0, best_usecount_c = 0;
        ll best_use_id1 = 0, best_use_id2 = 1, best_use_id3 = 1;
        ll best_dis_count = 0;
        Paint best_new_color(0, 0, 0, 0);

        ll qq = now_color.count / 6;
        ll rr = now_color.count % 6;

        // 最も重要な最適化：色の組み合わせのスキップ条件を追加
        rep(c1, k) {
            const Paint &color1 = own_color[c1];

            reps(c2, c1, k) { // c1+1 -> c1 に変更（同じ色も許可）
                const Paint &color2 = own_color[c2];

                reps(c3, c2, k) { // c2+1 -> c2 に変更
                    const Paint &color3 = own_color[c3];

                    // 早期スキップ条件：色が大きく違いすぎる場合は計算しない
                    double max_component_diff =
                        max({abs(color1.red - tr.red), abs(color1.green - tr.green),
                             abs(color1.blue - tr.blue), abs(color2.red - tr.red),
                             abs(color2.green - tr.green), abs(color2.blue - tr.blue),
                             abs(color3.red - tr.red), abs(color3.green - tr.green),
                             abs(color3.blue - tr.blue)});

                    if (max_component_diff > 0.8) continue; // 閾値による早期スキップ

                    rep(dis, min(6LL, now_color.count + 1)) { // 範囲最適化
                        ll use_now_color = (dis < rr ? qq + 1 : qq);

                        rep(ca, min(4LL, c + 1)) rep(cb, min(4LL, c + 1 - ca)) {
                            ll cc_max = min(3LL, c - ca - cb);
                            if (cc_max < 0) break; // 早期終了

                            rep(cc, cc_max + 1) {
                                if (ca + cb + cc + use_now_color < 1) continue;

                                ll sum = ca + cb + cc + use_now_color;

                                // 色計算を効率化
                                double inv_sum = 1.0 / sum;
                                double r       = (color1.red * ca + color2.red * cb + color3.red * cc +
                                            now_color.red * use_now_color) *
                                           inv_sum;
                                double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                            now_color.green * use_now_color) *
                                           inv_sum;
                                double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                            now_color.blue * use_now_color) *
                                           inv_sum;

                                // sqrt計算を最後まで遅延
                                double error_sq = calc.calc_error_sq(r, g, b);
                                double penalty  = (ca + cb + cc + now_color.count - use_now_color) * d;
                                double total_error = sqrt(error_sq) * 10000 + penalty;

                                // 早期終了：明らかに悪い解はスキップ
                                if (total_error >= best_error * 1.1) continue;

                                if (total_error < best_error) {
                                    best_error      = total_error;
                                    best_usecount_a = ca;
                                    best_usecount_b = cb;
                                    best_usecount_c = cc;
                                    best_use_id1    = c1;
                                    best_use_id2    = c2;
                                    best_use_id3    = c3;
                                    best_new_color  = Paint(r, g, b, sum);
                                    best_dis_count  = now_color.count - use_now_color;
                                }
                            }
                        }
                    }
                }
            }
        }

        now_color = best_new_color;

        // 出力部分は変更なし
        rep(i, best_dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, best_usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id1 << el;
        rep(i, best_usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id2 << el;
        rep(i, best_usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }

    return 0;
}