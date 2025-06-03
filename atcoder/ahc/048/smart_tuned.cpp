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

    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;  // **元の予算設定を維持**

    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &tr = target_color[hi];
        double error = 101010;

        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1, dis_count = 0;
        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;
        
        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];
            Paint &color3 = own_color[c3];

            ll use_now_color = -qq;
            rep(dis, 4) {
                use_now_color += (dis < rr ? qq + 1 : qq);

                // **元の探索範囲を維持（7×5×3）**
                rep(ca, 7) rep(cb, 5) rep(cc, 3) {
                    if (ca + cb + cc > c) continue;
                    if (ca + cb + cc + use_now_color < 1) continue;

                    ll sum = ca + cb + cc + use_now_color;

                    double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                now_color.red * use_now_color) / sum;
                    double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                now_color.green * use_now_color) / sum;
                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                now_color.blue * use_now_color) / sum;

                    double color_dist = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue));
                    
                    // **改善1: より精密な将来評価（dペナルティを考慮）**
                    double future_penalty = 0;
                    if (hi + 1 < h && sum > 1) {
                        Paint &next_tr = target_color[hi + 1];
                        Paint remaining(r, g, b, sum - 1);
                        double future_dist = sqrt(squ(remaining.red - next_tr.red) + 
                                                 squ(remaining.green - next_tr.green) + 
                                                 squ(remaining.blue - next_tr.blue));
                        // 将来の最小絵具コストを推定
                        double future_min_cost = max(1.0, future_dist * 30.0); // 経験的推定
                        future_penalty = future_min_cost * d * 0.1; // 軽い重み
                    }

                    // **改善2: 絵具効率を重視した評価**
                    double paint_efficiency = (ca + cb + cc) > 0 ? color_dist / (ca + cb + cc) : 1e9;
                    double efficiency_bonus = (paint_efficiency < 0.1) ? -d * 0.05 : 0; // 高効率にボーナス

                    double n_error = color_dist * 10000 + (ca + cb + cc) * d + future_penalty + efficiency_bonus;

                    if (chmin(error, n_error)) {
                        usecount_a = ca; usecount_b = cb; usecount_c = cc;
                        use_id1 = c1; use_id2 = c2; use_id3 = c3;
                        new_color.red = r; new_color.green = g; new_color.blue = b;
                        new_color.count = sum; dis_count = now_color.count - use_now_color;
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