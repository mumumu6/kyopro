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

auto squ = [](double x) { return x * x; };

// 超軽量先読み：最も近い単色のみチェック
double estimate_future_cost(const Paint& state, const Paint& target, const vector<Paint>& tubes, ll k, ll d) {
    if (state.count <= 0) return 1e6;
    
    double best_score = 1e9;
    
    // 各色について1個だけ追加した場合の最良スコアを計算
    rep(i, k) {
        double total = state.count + 1;
        double r = (state.red * state.count + tubes[i].red) / total;
        double g = (state.green * state.count + tubes[i].green) / total;
        double b = (state.blue * state.count + tubes[i].blue) / total;
        
        double dist = sqrt(squ(r - target.red) + squ(g - target.green) + squ(b - target.blue));
        double score = dist * 10000 + d; // 1個追加のコスト
        chmin(best_score, score);
    }
    
    // 何も追加しない場合も考慮
    if (state.count > 0) {
        double dist = sqrt(squ(state.red - target.red) + squ(state.green - target.green) + squ(state.blue - target.blue));
        chmin(best_score, dist * 10000);
    }
    
    return best_score;
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

                    // 現在のスコア（元のa2.cppと同じ）
                    double current_dist = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue));
                    double current_score = current_dist * 10000 + (ca + cb + cc) * d;
                    
                    // **軽量先読み：O(k)の追加コストのみ**
                    double future_penalty = 0;
                    if (hi + 1 < h && sum > 1) {
                        Paint next_state(r, g, b, sum - 1);
                        double future_cost = estimate_future_cost(next_state, target_color[hi + 1], own_color, k, d);
                        future_penalty = future_cost * 0.2; // 軽い重み
                    }

                    double total_score = current_score + future_penalty;

                    if (chmin(error, total_score)) {
                        usecount_a = ca; usecount_b = cb; usecount_c = cc;
                        use_id1 = c1; use_id2 = c2; use_id3 = c3;
                        new_color.red = r; new_color.green = g; new_color.blue = b;
                        new_color.count = sum; dis_count = now_color.count - use_now_color;
                    }
                }
            }
        }
        
        now_color = new_color;

        // 出力（元のa2.cppと完全に同じ）
        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
} 