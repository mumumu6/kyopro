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

// 🧠 プロ戦略（真の制約対応版）
struct ProStrategy {
    ll max_ca, max_cb, max_cc;
    ll color_limit;
    ll sa_iterations;
    bool enable_simulated_annealing;
    double efficiency_ratio;

    ProStrategy(ll k, ll h, ll t, ll d, TimeManager &timer) {
        // 機械学習最適値（H=1000、N=20固定での最適化）
        max_ca = 7;
        max_cb = 5;
        max_cc = 4;

        // 効率比計算（コスト／時間）
        efficiency_ratio = (double)d / t * 1000.0; // H=1000を考慮

        // K値による色選択戦略
        if (k <= 6) {
            // 少数精鋭：各色を有効活用
            color_limit   = k;
            max_ca        = 8;
            max_cb        = 6;
            max_cc        = 5;
            sa_iterations = 40;
        } else if (k <= 10) {
            // 中規模：バランス重視
            color_limit   = min(k, 7LL);
            max_ca        = 6;
            max_cb        = 5;
            max_cc        = 4;
            sa_iterations = 30;
        } else {
            // 大規模：選択的使用
            color_limit   = min(k, 6LL);
            max_ca        = 5;
            max_cb        = 4;
            max_cc        = 3;
            sa_iterations = 25;
        }

        // 効率比による調整
        if (efficiency_ratio > 1.0) {
            // 高コスト効率：絵具使用量を制限
            max_ca      = max(2LL, max_ca - 2);
            max_cb      = max(2LL, max_cb - 1);
            max_cc      = max(1LL, max_cc - 1);
            color_limit = max(3LL, color_limit - 1);
        } else if (efficiency_ratio < 0.3) {
            // 低コスト効率：品質重視
            max_ca      = min(10LL, max_ca + 2);
            max_cb      = min(8LL, max_cb + 1);
            max_cc      = min(6LL, max_cc + 1);
            color_limit = min(k, color_limit + 1);
        }

        // T値による時間調整
        if (t < 8000) {
            // 極少時間：最小限処理
            sa_iterations = 10;
            max_ca        = min(max_ca, 3LL);
            max_cb        = min(max_cb, 2LL);
            max_cc        = min(max_cc, 2LL);
            color_limit   = min(color_limit, 4LL);
        } else if (t > 50000) {
            // 十分時間：精度向上
            sa_iterations = min(60LL, sa_iterations + 20);
            if (efficiency_ratio < 0.5) {
                max_ca = min(12LL, max_ca + 3);
                max_cb = min(10LL, max_cb + 2);
                max_cc = min(8LL, max_cc + 2);
            }
        }

        enable_simulated_annealing = true;
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

        // 時間に応じた反復回数調整
        ll max_iter = strategy.sa_iterations;
        if (timer.remaining_ratio() < 0.5) {
            max_iter = max(5LL, max_iter / 2); // 時間不足なら半減
        }

        double temp_start = 1000.0, temp_end = 1.0;

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