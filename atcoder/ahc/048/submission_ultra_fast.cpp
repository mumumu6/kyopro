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

    // 🚀 超高速化：探索制限を極限まで削減
    ll max_colors = min(k, 8LL);           // 色は最大8色まで
    ll max_ca = 4, max_cb = 3, max_cc = 2; // 使用量も最小限

    // 大規模問題では更に削減
    if (t > 30000) {
        max_colors = min(k, 6LL);
        max_ca     = 3;
        max_cb     = 2;
        max_cc     = 2;
    }

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

        // 🚀 最小限の色探索（3重ループを大幅制限）
        rep(c1, min(k, max_colors)) {
            ll c2_max = min(k, c1 + max_colors / 2);
            reps(c2, c1 + 1, c2_max) {
                ll c3_max = min(k, c2 + max_colors / 3);
                reps(c3, c2 + 1, c3_max) {
                    Paint &color1 = own_color[c1];
                    Paint &color2 = own_color[c2];
                    Paint &color3 = own_color[c3];

                    ll use_now_color = -qq;
                    rep(dis, min(4LL, 2LL)) { // dis も制限
                        use_now_color += (dis < rr ? qq + 1 : qq);

                        // 🚀 使用量探索を最小限に
                        rep(ca, max_ca) {
                            rep(cb, max_cb) {
                                rep(cc, max_cc) {
                                    if (ca + cb + cc > c) break; // 早期終了
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