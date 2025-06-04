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

// ⏱️ 実行時間管理
class TimeManager {
  private:
    chrono::steady_clock::time_point start_time;
    double time_limit_ms;

  public:
    TimeManager(double limit_ms = 1900.0) : time_limit_ms(limit_ms) {
        start_time = chrono::steady_clock::now();
    }

    double elapsed_ms() const {
        auto now = chrono::steady_clock::now();
        return chrono::duration<double, milli>(now - start_time).count();
    }

    bool is_time_up() const { return elapsed_ms() > time_limit_ms; }

    double remaining_ratio() const { return max(0.0, (time_limit_ms - elapsed_ms()) / time_limit_ms); }
};

// 🧠 プロ戦略（時間適応型）
struct ProStrategy {
    ll max_ca, max_cb, max_cc;
    ll color_limit;
    ll beam_width;
    ll sa_iterations;
    bool enable_beam_search;
    bool enable_simulated_annealing;

    ProStrategy(ll k, ll h, ll t, ll d, TimeManager &timer) {
        // 基本ML結果
        max_ca = 7;
        max_cb = 5;
        max_cc = 4;

        // 制約による動的調整
        if (h >= 800) {
            // 超大規模：高速化優先
            max_ca                     = 3;
            max_cb                     = 2;
            max_cc                     = 2;
            color_limit                = min(k, 4LL);
            beam_width                 = 3;
            sa_iterations              = 20;
            enable_beam_search         = false;
            enable_simulated_annealing = true;
        } else if (h >= 200) {
            // 大規模：バランス
            max_ca                     = 5;
            max_cb                     = 4;
            max_cc                     = 3;
            color_limit                = min(k, 6LL);
            beam_width                 = 4;
            sa_iterations              = 30;
            enable_beam_search         = true;
            enable_simulated_annealing = true;
        } else {
            // 小中規模：精度優先
            color_limit                = min(k, 8LL);
            beam_width                 = 6;
            sa_iterations              = 50;
            enable_beam_search         = true;
            enable_simulated_annealing = true;
        }

        // コスト効率調整
        if (d > 1000) {
            max_ca = max(1LL, max_ca - 1);
            max_cb = max(1LL, max_cb - 1);
            max_cc = max(1LL, max_cc - 1);
        }

        // 時間による更なる調整
        if (timer.elapsed_ms() < 100) { // 初期段階
            if (t > 50000) {
                beam_width    = max(2LL, beam_width - 1);
                sa_iterations = max(15LL, sa_iterations - 10);
            }
        }
    }
};

// 🎯 解構造体
struct Solution {
    vector<ll> amounts = vector<ll>(3, 0);
    vector<ll> colors  = vector<ll>(3, 0);
    ll discard         = 0;
    double score       = 1e9;
    Paint result;

    Solution() {}
};

// 🎨 高速色選択
vector<int> select_colors_fast(const vector<Paint> &colors, int limit) {
    int k = colors.size();
    if (k <= limit) {
        vector<int> all(k);
        iota(all.begin(), all.end(), 0);
        return all;
    }

    vector<int> selected;

    // RGB軸近似色
    array<pair<double, ll>, 3> axis_candidates;
    rep(i, 3) axis_candidates[i] = {1e9, 0};

    rep(i, k) {
        double d_r = colors[i].green * colors[i].green + colors[i].blue * colors[i].blue;
        double d_g = colors[i].red * colors[i].red + colors[i].blue * colors[i].blue;
        double d_b = colors[i].red * colors[i].red + colors[i].green * colors[i].green;

        chmin(axis_candidates[0], make_pair(d_r, (ll)i));
        chmin(axis_candidates[1], make_pair(d_g, (ll)i));
        chmin(axis_candidates[2], make_pair(d_b, (ll)i));
    }

    set<int> used;
    for (auto &p : axis_candidates) {
        if (used.find((int)p.second) == used.end()) {
            selected.push_back((int)p.second);
            used.insert((int)p.second);
        }
    }

    // ランダム追加（多様性確保）
    random_device rd;
    mt19937 rng(rd());
    while ((int)selected.size() < limit && (int)selected.size() < k) {
        int candidate = uniform_int_distribution<int>(0, k - 1)(rng);
        if (used.find(candidate) == used.end()) {
            selected.push_back(candidate);
            used.insert(candidate);
        }
    }

    return selected;
}

// 🔥 高速焼きなまし
class FastSA {
  private:
    mt19937 rng;
    ProStrategy strategy;
    vector<Paint> colors;
    vector<int> mapping;
    ll d_cost;

  public:
    FastSA(const ProStrategy &s, const vector<Paint> &c, const vector<int> &m, ll d)
        : strategy(s), colors(c), mapping(m), d_cost(d) {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    Solution solve(const Paint &target, const Paint &current, ll budget, TimeManager &timer) {
        if (!strategy.enable_simulated_annealing) { return greedy_solve(target, current, budget); }

        Solution best = generate_initial(target, current, budget);
        Solution cur  = best;

        ll max_iter       = min(strategy.sa_iterations, (ll)(timer.remaining_ratio() * 100));
        double temp_start = 100.0, temp_end = 1.0;

        rep(iter, max_iter) {
            if (timer.is_time_up()) break;

            double temp       = temp_start * pow(temp_end / temp_start, (double)iter / max_iter);
            Solution neighbor = mutate(cur, target, current, budget);

            double delta = neighbor.score - cur.score;
            if (delta < 0 || uniform_real_distribution<double>(0, 1)(rng) < exp(-delta / temp)) {
                cur = neighbor;
                if (cur.score < best.score) best = cur;
            }
        }

        return best;
    }

  private:
    Solution generate_initial(const Paint &target, const Paint &current, ll budget) {
        Solution sol;

        // 簡単な初期化
        rep(i, 3) {
            sol.colors[i]  = uniform_int_distribution<int>(0, (int)colors.size() - 1)(rng);
            sol.amounts[i] = uniform_int_distribution<ll>(0, min(budget / 3, strategy.max_ca))(rng);
        }

        evaluate(sol, target, current);
        return sol;
    }

    Solution mutate(const Solution &sol, const Paint &target, const Paint &current, ll budget) {
        Solution mut = sol;

        int type = uniform_int_distribution<int>(0, 1)(rng);
        if (type == 0) {
            // 色変更
            int idx         = uniform_int_distribution<int>(0, 2)(rng);
            mut.colors[idx] = uniform_int_distribution<int>(0, (int)colors.size() - 1)(rng);
        } else {
            // 量調整
            int idx          = uniform_int_distribution<int>(0, 2)(rng);
            int delta        = uniform_int_distribution<int>(-1, 1)(rng);
            mut.amounts[idx] = max(0LL, mut.amounts[idx] + delta);
        }

        evaluate(mut, target, current);
        return mut;
    }

    Solution greedy_solve(const Paint &target, const Paint &current, ll budget) {
        Solution best;
        best.score = 1e9;

        ll max_colors = min((ll)colors.size(), 3LL);
        rep(c1, max_colors) rep(c2, max_colors) rep(c3, max_colors) {
            rep(a1, min(strategy.max_ca + 1, budget + 1)) {
                rep(a2, min(strategy.max_cb + 1, budget + 1 - a1)) {
                    ll a3_max = min(strategy.max_cc, budget - a1 - a2);
                    if (a3_max < 0) continue;

                    rep(a3, a3_max + 1) {
                        Solution sol;
                        sol.colors  = {c1, c2, c3};
                        sol.amounts = {a1, a2, a3};
                        evaluate(sol, target, current);

                        if (sol.score < best.score) best = sol;
                    }
                }
            }
        }

        return best;
    }

    void evaluate(Solution &sol, const Paint &target, const Paint &current) {
        ll total       = sol.amounts[0] + sol.amounts[1] + sol.amounts[2];
        ll current_use = max(0LL, current.count - sol.discard);
        ll sum         = total + current_use;

        if (sum <= 0) {
            sol.score = 1e9;
            return;
        }

        double r = 0, g = 0, b = 0;
        rep(i, 3) {
            r += colors[sol.colors[i]].red * sol.amounts[i];
            g += colors[sol.colors[i]].green * sol.amounts[i];
            b += colors[sol.colors[i]].blue * sol.amounts[i];
        }

        if (current_use > 0) {
            r += current.red * current_use;
            g += current.green * current_use;
            b += current.blue * current_use;
        }

        r /= sum;
        g /= sum;
        b /= sum;

        double dr = r - target.red;
        double dg = g - target.green;
        double db = b - target.blue;

        sol.score  = sqrt(dr * dr + dg * dg + db * db) * 10000 + total * d_cost;
        sol.result = Paint(r, g, b, sum);
    }
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    TimeManager timer(1800.0); // 1.8秒制限

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k);
    vector<Paint> target_color(h);

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    // 🧠 プロ戦略決定
    ProStrategy strategy(k, h, t, d, timer);

    // 🎨 色選択
    vector<int> selected = select_colors_fast(own_color, strategy.color_limit);
    vector<Paint> selected_colors(selected.size());
    rep(i, selected.size()) selected_colors[i] = own_color[selected[i]];

    // パレット初期化
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    // 🔥 AIソルバー初期化
    FastSA solver(strategy, selected_colors, selected, d);

    ll c = (t / h) / 2;
    Paint current_color(0, 0, 0, 0);

    rep(hi, h) {
        if (timer.is_time_up()) {
            // 時間切れ：デフォルト動作
            cout << 2 << spa << 0 << spa << 0 << el;
            continue;
        }

        Paint &target = target_color[hi];

        Solution best = solver.solve(target, current_color, c, timer);
        current_color = best.result;

        // アクション出力
        rep(i, best.discard) cout << 3 << spa << 0 << spa << 0 << el;
        rep(i, best.amounts[0]) cout << 1 << spa << 0 << spa << 0 << spa << selected[best.colors[0]]
                                     << el;
        rep(i, best.amounts[1]) cout << 1 << spa << 0 << spa << 0 << spa << selected[best.colors[1]]
                                     << el;
        rep(i, best.amounts[2]) cout << 1 << spa << 0 << spa << 0 << spa << selected[best.colors[2]]
                                     << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        current_color.count--;
    }

    return 0;
}