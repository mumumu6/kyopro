#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
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

// 軽量版将来スコア計算（代表的な組み合わせのみ）
double quick_future_estimate(const Paint& state, const Paint& target, ll k, const vector<Paint>& tubes, ll d) {
    if (state.count <= 0) return 1e6;
    
    double best_score = 1e9;
    
    // 単色のみで高速評価
    rep(i, k) {
        rep(amount, 4) { // 1-3個のみ
            if (amount == 0) continue;
            
            double total = state.count + amount;
            double r = (state.red * state.count + tubes[i].red * amount) / total;
            double g = (state.green * state.count + tubes[i].green * amount) / total;
            double b = (state.blue * state.count + tubes[i].blue * amount) / total;
            
            double dist = sqrt(squ(r - target.red) + squ(g - target.green) + squ(b - target.blue));
            double score = dist * 10000 + amount * d;
            chmin(best_score, score);
        }
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

    ll c = (t / h) / 2;
    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &tr = target_color[hi];
        double error = 1e9;

        ll best_usecount_a = 0, best_usecount_b = 0, best_usecount_c = 0;
        ll best_use_id1 = 0, best_use_id2 = 1, best_use_id3 = 1, best_dis_count = 0;
        Paint best_new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;
        
        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            ll use_now_color = -qq;
            rep(dis, 4) {
                use_now_color += (dis < rr ? qq + 1 : qq);

                rep(ca, 7) rep(cb, 5) rep(cc, 3) {
                    if (ca + cb + cc > c) continue;
                    if (ca + cb + cc + use_now_color < 1) continue;

                    ll sum = ca + cb + cc + use_now_color;

                    double r = (own_color[c1].red * ca + own_color[c2].red * cb + own_color[c3].red * cc +
                                now_color.red * use_now_color) / sum;
                    double g = (own_color[c1].green * ca + own_color[c2].green * cb + own_color[c3].green * cc +
                                now_color.green * use_now_color) / sum;
                    double b = (own_color[c1].blue * ca + own_color[c2].blue * cb + own_color[c3].blue * cc +
                                now_color.blue * use_now_color) / sum;

                    // 現在のスコア
                    double current_dist = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue));
                    double current_score = current_dist * 10000 + (ca + cb + cc) * d;
                    
                    // 将来スコア（軽量版）
                    double future_score = 0;
                    if (hi + 1 < h) {
                        Paint next_state(r, g, b, max(0LL, sum - 1));
                        future_score = quick_future_estimate(next_state, target_color[hi + 1], k, own_color, d) * 0.3;
                    }
                    
                    double total_score = current_score + future_score;

                    if (chmin(error, total_score)) {
                        best_usecount_a = ca; best_usecount_b = cb; best_usecount_c = cc;
                        best_use_id1 = c1; best_use_id2 = c2; best_use_id3 = c3;
                        best_new_color.red = r; best_new_color.green = g; best_new_color.blue = b;
                        best_new_color.count = sum; best_dis_count = now_color.count - use_now_color;
                    }
                }
            }
        }
        
        now_color = best_new_color;

        rep(i, best_dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, best_usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id1 << el;
        rep(i, best_usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id2 << el;
        rep(i, best_usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
} 