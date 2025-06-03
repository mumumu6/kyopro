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
    double count = 1;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0) : red(r), green(g), blue(b), count(c) {}
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

    // パレット設定を簡素化
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &tr = target_color[hi];
        double error = 1010101;
        
        ll best_c1 = 0, best_c2 = 0, best_c3 = 0;
        ll best_ca = 0, best_cb = 0, best_cc = 0;

        // **改善点1: 探索範囲を7^3から12^3に拡張**
        rep(c1, k) reps(c2, c1, k) reps(c3, c2, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];
            Paint &color3 = own_color[c3];

            // **改善点2: より細かい比率で探索（7→12）**
            rep(ca, 13) rep(cb, 13) rep(cc, 13) {
                if (ca + cb + cc == 0) continue;
                if (ca + cb + cc > 20) continue; // **改善点3: 上限を緩和**

                double sum = ca + cb + cc + now_color.count;
                if (sum < 1e-9) continue;

                double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                            now_color.red * now_color.count) / sum;
                double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                            now_color.green * now_color.count) / sum;
                double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                            now_color.blue * now_color.count) / sum;

                double color_error = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue));
                
                // **改善点4: 将来の色も考慮した評価**
                double future_penalty = 0;
                if (hi + 1 < h) {
                    Paint &next_target = target_color[hi + 1];
                    Paint remaining(r, g, b, max(0.0, sum - 1.0));
                    if (remaining.count > 1e-9) {
                        future_penalty = sqrt(squ(remaining.red - next_target.red) + 
                                             squ(remaining.green - next_target.green) + 
                                             squ(remaining.blue - next_target.blue)) * 500; // 軽い重み
                    }
                }

                double total_error = color_error * 10000 + (ca + cb + cc) * d + future_penalty;

                if (chmin(error, total_error)) {
                    best_c1 = c1; best_c2 = c2; best_c3 = c3;
                    best_ca = ca; best_cb = cb; best_cc = cc;
                }
            }
        }

        // 絵具追加
        rep(i, best_ca) cout << 1 << spa << 0 << spa << 0 << spa << best_c1 << el;
        rep(i, best_cb) cout << 1 << spa << 0 << spa << 0 << spa << best_c2 << el;
        rep(i, best_cc) cout << 1 << spa << 0 << spa << 0 << spa << best_c3 << el;

        // 色取り出し
        cout << 2 << spa << 0 << spa << 0 << el;

        // 残り絵具を正確に計算
        double sum = best_ca + best_cb + best_cc + now_color.count;
        if (sum > 1e-9) {
            now_color.red = (own_color[best_c1].red * best_ca + 
                           own_color[best_c2].red * best_cb + 
                           own_color[best_c3].red * best_cc +
                           now_color.red * now_color.count) / sum;
            now_color.green = (own_color[best_c1].green * best_ca + 
                             own_color[best_c2].green * best_cb + 
                             own_color[best_c3].green * best_cc +
                             now_color.green * now_color.count) / sum;
            now_color.blue = (own_color[best_c1].blue * best_ca + 
                            own_color[best_c2].blue * best_cb + 
                            own_color[best_c3].blue * best_cc +
                            now_color.blue * now_color.count) / sum;
            now_color.count = max(0.0, sum - 1.0);
        } else {
            now_color = Paint(0, 0, 0, 0);
        }
    }
} 