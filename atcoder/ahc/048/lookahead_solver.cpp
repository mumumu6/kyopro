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

// 2手先の最適スコアを計算する関数
double calculate_future_score(const Paint& current_state, int target_idx, 
                             const vector<Paint>& own_colors, const vector<Paint>& targets, 
                             ll k, ll d, ll budget) {
    if (target_idx >= targets.size()) return 0;
    
    const Paint& target = targets[target_idx];
    double best_score = 1e9;
    
    // 元のa2.cppと同じ探索範囲で高速計算
    ll qq = current_state.count / 4;
    ll rr = current_state.count % 4;
    
    rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
        const Paint &color1 = own_colors[c1];
        const Paint &color2 = own_colors[c2];
        const Paint &color3 = own_colors[c3];

        ll use_now_color = -qq;
        rep(dis, 4) {
            use_now_color += (dis < rr ? qq + 1 : qq);

            // 探索範囲を少し絞って高速化
            rep(ca, min(7LL, budget)) rep(cb, min(5LL, budget)) rep(cc, min(3LL, budget)) {
                if (ca + cb + cc > budget) continue;
                if (ca + cb + cc + use_now_color < 1) continue;

                ll sum = ca + cb + cc + use_now_color;

                double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                            current_state.red * use_now_color) / sum;
                double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                            current_state.green * use_now_color) / sum;
                double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                            current_state.blue * use_now_color) / sum;

                double color_dist = sqrt(squ(r - target.red) + squ(g - target.green) + squ(b - target.blue));
                double score = color_dist * 10000 + (ca + cb + cc) * d;
                
                chmin(best_score, score);
            }
        }
    }
    
    return best_score;
}

// 3手先を見越した評価関数
double evaluate_with_lookahead(const Paint& current_state, int target_idx,
                               const vector<Paint>& own_colors, const vector<Paint>& targets,
                               ll k, ll d, ll budget,
                               ll ca, ll cb, ll cc, ll c1, ll c2, ll c3, ll use_now_color) {
    
    ll sum = ca + cb + cc + use_now_color;
    if (sum <= 0) return 1e9;
    
    // 現在の色を作成
    double r = (own_colors[c1].red * ca + own_colors[c2].red * cb + own_colors[c3].red * cc +
                current_state.red * use_now_color) / sum;
    double g = (own_colors[c1].green * ca + own_colors[c2].green * cb + own_colors[c3].green * cc +
                current_state.green * use_now_color) / sum;
    double b = (own_colors[c1].blue * ca + own_colors[c2].blue * cb + own_colors[c3].blue * cc +
                current_state.blue * use_now_color) / sum;

    // 現在のターゲットに対するスコア
    double current_dist = sqrt(squ(r - targets[target_idx].red) + 
                              squ(g - targets[target_idx].green) + 
                              squ(b - targets[target_idx].blue));
    double current_score = current_dist * 10000 + (ca + cb + cc) * d;
    
    // 残り絵具で次のターゲットを予測
    Paint next_state(r, g, b, max(0LL, sum - 1));
    double future_score1 = (target_idx + 1 < targets.size()) ? 
        calculate_future_score(next_state, target_idx + 1, own_colors, targets, k, d, budget) : 0;
    
    // さらに次のターゲットも軽く評価
    double future_score2 = 0;
    if (target_idx + 2 < targets.size() && next_state.count > 0) {
        Paint next_next_state(next_state.red, next_state.green, next_state.blue, max(0LL, next_state.count - 1));
        future_score2 = calculate_future_score(next_next_state, target_idx + 2, own_colors, targets, k, d, budget) * 0.3;
    }
    
    return current_score + future_score1 * 0.4 + future_score2;
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

    // パレット設定（元のa2.cppと同じ）
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;  // 元の予算設定
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

                    // **先読み評価を使用**
                    double total_score = evaluate_with_lookahead(
                        now_color, hi, own_color, target_color, k, d, c,
                        ca, cb, cc, c1, c2, c3, use_now_color
                    );

                    if (chmin(error, total_score)) {
                        best_usecount_a = ca; best_usecount_b = cb; best_usecount_c = cc;
                        best_use_id1 = c1; best_use_id2 = c2; best_use_id3 = c3;
                        
                        ll sum = ca + cb + cc + use_now_color;
                        best_new_color.red = (own_color[c1].red * ca + own_color[c2].red * cb + 
                                             own_color[c3].red * cc + now_color.red * use_now_color) / sum;
                        best_new_color.green = (own_color[c1].green * ca + own_color[c2].green * cb + 
                                               own_color[c3].green * cc + now_color.green * use_now_color) / sum;
                        best_new_color.blue = (own_color[c1].blue * ca + own_color[c2].blue * cb + 
                                              own_color[c3].blue * cc + now_color.blue * use_now_color) / sum;
                        best_new_color.count = sum;
                        best_dis_count = now_color.count - use_now_color;
                    }
                }
            }
        }
        
        now_color = best_new_color;

        // 出力（元のa2.cppと同じフォーマット）
        rep(i, best_dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, best_usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id1 << el;
        rep(i, best_usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id2 << el;
        rep(i, best_usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << best_use_id3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
} 