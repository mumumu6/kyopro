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

// 🚀 TLE対策：大幅高速化パラメータ
struct FastParams {
    ll max_ca, max_cb, max_cc;
    ll max_colors; // 色探索制限

    FastParams(ll k, ll h, ll t, ll d) {
        // 時間制限対策で大幅削減
        max_ca = 5; // 7→5
        max_cb = 4; // 5→4
        max_cc = 3; // 4→3

        // 色探索も制限（最大の高速化ポイント）
        max_colors = min(k, 12LL); // 最大12色まで

        // 問題サイズに応じて更に調整
        if (t > 40000) {
            // 大規模問題：更に削減
            max_ca     = 4;
            max_cb     = 3;
            max_cc     = 2;
            max_colors = min(k, 10LL);
        } else if (t < 10000) {
            // 小規模問題：少し余裕を持たせる
            max_ca     = 6;
            max_cb     = 4;
            max_cc     = 3;
            max_colors = min(k, 15LL);
        }

        // コスト効率で更に調整
        double cost_ratio = (double)d / t;
        if (cost_ratio > 0.3) {
            max_ca = max(1LL, max_ca - 1);
            max_cb = max(1LL, max_cb - 1);
            max_cc = max(1LL, max_cc - 1);
        }
    }
};

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

    // 🚀 高速化パラメータ適用
    FastParams fast_params(k, h, t, d);

    auto squ = [](double x) { return x * x; };

    // パレット初期化（n=20固定）
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

    // h=1000の高速処理
    rep(hi, h) {
        Paint &tr    = target_color[hi];
        double error = 101010;

        ll usecount_a = 0;
        ll usecount_b = 0;
        ll usecount_c = 0;

        ll use_id1   = 0;
        ll use_id2   = 1;
        ll use_id3   = 1;
        ll dis_count = 0;

        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;

        // 🚀 色探索を大幅制限（最大の高速化）
        ll color_limit = fast_params.max_colors;
        rep(c1, min(k, color_limit)) {
            reps(c2, c1 + 1, min(k, c1 + color_limit / 2)) {
                reps(c3, c2 + 1, min(k, c2 + color_limit / 3)) {
                    Paint &color1 = own_color[c1];
                    Paint &color2 = own_color[c2];
                    Paint &color3 = own_color[c3];

                    ll use_now_color = -qq;
                    rep(dis, 4) {
                        use_now_color += (dis < rr ? qq + 1 : qq);

                        // 🚀 使用量探索も制限
                        rep(ca, fast_params.max_ca) {
                            rep(cb, fast_params.max_cb) {
                                rep(cc, fast_params.max_cc) {
                                    if (ca + cb + cc > c) continue;
                                    if (ca + cb + cc + use_now_color < 1) continue;

                                    ll sum = ca + cb + cc + use_now_color;

                                    double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                                now_color.red * use_now_color) /
                                               sum;
                                    double g = (color1.green * ca + color2.green * cb +
                                                color3.green * cc + now_color.green * use_now_color) /
                                               sum;
                                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                                now_color.blue * use_now_color) /
                                               sum;

                                    double n_error =
                                        sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) *
                                            10000 +
                                        (ca + cb + cc) * d;

                                    if (chmin(error, n_error)) {
                                        usecount_a = ca;
                                        usecount_b = cb;
                                        usecount_c = cc;

                                        use_id1         = c1;
                                        use_id2         = c2;
                                        use_id3         = c3;
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