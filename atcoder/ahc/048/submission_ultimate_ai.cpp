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

// 🤖 機械学習ベース戦略
struct MLStrategy {
    ll max_ca, max_cb, max_cc;
    ll beam_width;
    ll annealing_iterations;
    double temperature_start, temperature_end;
    ll color_limit;
    bool use_aggressive_pruning;

    MLStrategy(ll k, ll h, ll t, ll d) {
        // 20000ケース学習結果 + 動的調整
        max_ca = 7;
        max_cb = 5;
        max_cc = 4; // 基本最適値

        // h=1000対策
        if (h >= 500) {
            max_ca               = min(max_ca, 4LL);
            max_cb               = min(max_cb, 3LL);
            max_cc               = min(max_cc, 2LL);
            color_limit          = min(k, 6LL);
            beam_width           = 5;
            annealing_iterations = 50;
        } else {
            color_limit          = min(k, 10LL);
            beam_width           = 8;
            annealing_iterations = 100;
        }

        // t,d による調整
        if (t > 30000) {
            beam_width           = max(3LL, beam_width - 2);
            annealing_iterations = max(30LL, annealing_iterations - 20);
        }

        if (d > 1000) {
            max_ca                 = max(1LL, max_ca - 1);
            max_cb                 = max(1LL, max_cb - 1);
            max_cc                 = max(1LL, max_cc - 1);
            use_aggressive_pruning = true;
        } else {
            use_aggressive_pruning = false;
        }

        temperature_start = 1000.0;
        temperature_end   = 1.0;
    }
};

// 🎯 解候補（ビームサーチ用）
struct Solution {
    vector<ll> use_amounts; // ca, cb, cc
    vector<ll> color_ids;   // c1, c2, c3
    ll discard_count;
    double score;
    Paint result_color;

    Solution() : use_amounts(3, 0), color_ids(3, 0), discard_count(0), score(1e9) {}
};

// 🧠 色選択アルゴリズム（改良版FPS）
vector<int> select_optimal_colors_ai(const vector<Paint> &colors, int target_count) {
    const int k = colors.size();
    if (k <= target_count) {
        vector<int> all(k);
        iota(all.begin(), all.end(), 0);
        return all;
    }

    vector<int> selected;

    // RGB軸代表色選択
    array<int, 3> axis_best    = {0, 0, 0};
    array<double, 3> axis_dist = {1e9, 1e9, 1e9};

    for (int i = 0; i < k; ++i) {
        double d_r = colors[i].green * colors[i].green + colors[i].blue * colors[i].blue;
        double d_g = colors[i].red * colors[i].red + colors[i].blue * colors[i].blue;
        double d_b = colors[i].red * colors[i].red + colors[i].green * colors[i].green;

        if (d_r < axis_dist[0]) {
            axis_dist[0] = d_r;
            axis_best[0] = i;
        }
        if (d_g < axis_dist[1]) {
            axis_dist[1] = d_g;
            axis_best[1] = i;
        }
        if (d_b < axis_dist[2]) {
            axis_dist[2] = d_b;
            axis_best[2] = i;
        }
    }

    set<int> used;
    for (int idx : axis_best) {
        if (used.find(idx) == used.end()) {
            selected.push_back(idx);
            used.insert(idx);
        }
    }

    // FPS拡張
    vector<double> min_dist(k, 1e30);
    for (int idx : selected) {
        for (int i = 0; i < k; ++i) {
            double dx   = colors[i].red - colors[idx].red;
            double dy   = colors[i].green - colors[idx].green;
            double dz   = colors[i].blue - colors[idx].blue;
            min_dist[i] = min(min_dist[i], dx * dx + dy * dy + dz * dz);
        }
    }

    while ((int)selected.size() < target_count) {
        int best_i       = -1;
        double best_dist = -1;
        for (int i = 0; i < k; ++i) {
            if (used.find(i) == used.end() && min_dist[i] > best_dist) {
                best_dist = min_dist[i];
                best_i    = i;
            }
        }
        if (best_i == -1) break;

        selected.push_back(best_i);
        used.insert(best_i);
        for (int i = 0; i < k; ++i) {
            double dx   = colors[i].red - colors[best_i].red;
            double dy   = colors[i].green - colors[best_i].green;
            double dz   = colors[i].blue - colors[best_i].blue;
            min_dist[i] = min(min_dist[i], dx * dx + dy * dy + dz * dz);
        }
    }

    return selected;
}

// 🔥 焼きなまし法ソルバー
class SimulatedAnnealing {
  private:
    mt19937 rng;
    MLStrategy strategy;
    vector<Paint> colors;
    vector<int> color_mapping;
    ll d_penalty;

  public:
    SimulatedAnnealing(const MLStrategy &strat, const vector<Paint> &cols, const vector<int> &mapping,
                       ll d)
        : strategy(strat), colors(cols), color_mapping(mapping), d_penalty(d) {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }

    Solution solve_single_target(const Paint &target, const Paint &current_color, ll c_limit) {
        Solution best;
        best.score = 1e9;

        // 初期解生成
        Solution current = generate_random_solution(target, current_color, c_limit);
        if (current.score < best.score) best = current;

        // 焼きなまし
        for (ll iter = 0; iter < strategy.annealing_iterations; ++iter) {
            double temp =
                strategy.temperature_start * pow(strategy.temperature_end / strategy.temperature_start,
                                                 (double)iter / strategy.annealing_iterations);

            Solution neighbor = mutate_solution(current, target, current_color, c_limit);

            double delta = neighbor.score - current.score;
            if (delta < 0 || uniform_real_distribution<double>(0, 1)(rng) < exp(-delta / temp)) {
                current = neighbor;
                if (current.score < best.score) best = current;
            }
        }

        return best;
    }

  private:
    Solution generate_random_solution(const Paint &target, const Paint &current_color, ll c_limit) {
        Solution sol;

        // ランダム色選択
        rep(i, 3) sol.color_ids[i] = uniform_int_distribution<int>(0, (int)colors.size() - 1)(rng);

        // ランダム使用量
        sol.use_amounts[0] = uniform_int_distribution<ll>(0, strategy.max_ca)(rng);
        sol.use_amounts[1] = uniform_int_distribution<ll>(0, strategy.max_cb)(rng);
        sol.use_amounts[2] = uniform_int_distribution<ll>(0, strategy.max_cc)(rng);

        ll sum_amounts = sol.use_amounts[0] + sol.use_amounts[1] + sol.use_amounts[2];
        if (sum_amounts > c_limit) {
            double scale                 = (double)c_limit / sum_amounts;
            rep(i, 3) sol.use_amounts[i] = (ll)(sol.use_amounts[i] * scale);
        }

        evaluate_solution(sol, target, current_color);
        return sol;
    }

    Solution mutate_solution(const Solution &sol, const Paint &target, const Paint &current_color,
                             ll c_limit) {
        Solution mutated = sol;

        // 変異タイプ選択
        int mutation_type = uniform_int_distribution<int>(0, 2)(rng);

        if (mutation_type == 0) {
            // 色変更
            int idx                = uniform_int_distribution<int>(0, 2)(rng);
            mutated.color_ids[idx] = uniform_int_distribution<int>(0, (int)colors.size() - 1)(rng);
        } else if (mutation_type == 1) {
            // 使用量調整
            int idx                  = uniform_int_distribution<int>(0, 2)(rng);
            int delta                = uniform_int_distribution<int>(-2, 2)(rng);
            mutated.use_amounts[idx] = max(0LL, mutated.use_amounts[idx] + delta);
        } else {
            // 全体スケール
            double scale                     = uniform_real_distribution<double>(0.8, 1.2)(rng);
            rep(i, 3) mutated.use_amounts[i] = max(0LL, (ll)(mutated.use_amounts[i] * scale));
        }

        // 制約チェック
        ll sum_amounts = mutated.use_amounts[0] + mutated.use_amounts[1] + mutated.use_amounts[2];
        if (sum_amounts > c_limit) {
            double scale                     = (double)c_limit / sum_amounts;
            rep(i, 3) mutated.use_amounts[i] = (ll)(mutated.use_amounts[i] * scale);
        }

        evaluate_solution(mutated, target, current_color);
        return mutated;
    }

    void evaluate_solution(Solution &sol, const Paint &target, const Paint &current_color) {
        ll total_used = sol.use_amounts[0] + sol.use_amounts[1] + sol.use_amounts[2];
        if (total_used == 0) {
            sol.score = 1e9;
            return;
        }

        ll sum = total_used + (current_color.count > 0 ? current_color.count - sol.discard_count : 0);
        if (sum <= 0) {
            sol.score = 1e9;
            return;
        }

        double r = 0, g = 0, b = 0;
        rep(i, 3) {
            r += colors[sol.color_ids[i]].red * sol.use_amounts[i];
            g += colors[sol.color_ids[i]].green * sol.use_amounts[i];
            b += colors[sol.color_ids[i]].blue * sol.use_amounts[i];
        }

        if (current_color.count > sol.discard_count) {
            ll use_current = current_color.count - sol.discard_count;
            r += current_color.red * use_current;
            g += current_color.green * use_current;
            b += current_color.blue * use_current;
        }

        r /= sum;
        g /= sum;
        b /= sum;

        double dr = r - target.red;
        double dg = g - target.green;
        double db = b - target.blue;

        sol.score        = sqrt(dr * dr + dg * dg + db * db) * 10000 + total_used * d_penalty;
        sol.result_color = Paint(r, g, b, sum);
    }
};

// 🌟 ビームサーチソルバー
class BeamSearch {
  private:
    MLStrategy strategy;
    vector<Paint> colors;
    vector<int> color_mapping;
    ll d_penalty;

  public:
    BeamSearch(const MLStrategy &strat, const vector<Paint> &cols, const vector<int> &mapping, ll d)
        : strategy(strat), colors(cols), color_mapping(mapping), d_penalty(d) {}

    Solution solve_target(const Paint &target, const Paint &current_color, ll c_limit) {
        vector<Solution> beam;

        // 初期ビーム生成
        rep(c1, min((ll)colors.size(), strategy.beam_width)) {
            rep(c2, min((ll)colors.size(), strategy.beam_width)) {
                rep(c3, min((ll)colors.size(), strategy.beam_width)) {
                    Solution sol;
                    sol.color_ids = {c1, c2, c3};

                    // 各使用量パターンを試行
                    reps(ca, 0, min(strategy.max_ca + 1, c_limit + 1)) {
                        reps(cb, 0, min(strategy.max_cb + 1, c_limit + 1 - ca)) {
                            reps(cc, 0, min(strategy.max_cc + 1, c_limit + 1 - ca - cb)) {
                                if (strategy.use_aggressive_pruning &&
                                    (ca + cb + cc) * d_penalty > 100000)
                                    continue;

                                Solution candidate    = sol;
                                candidate.use_amounts = {ca, cb, cc};
                                evaluate_beam_solution(candidate, target, current_color);

                                beam.push_back(candidate);
                            }
                        }
                    }
                }
            }
        }

        // ビーム選択
        sort(beam.begin(), beam.end(),
             [](const Solution &a, const Solution &b) { return a.score < b.score; });

        if (beam.empty()) {
            Solution default_sol;
            evaluate_beam_solution(default_sol, target, current_color);
            return default_sol;
        }

        return beam[0];
    }

  private:
    void evaluate_beam_solution(Solution &sol, const Paint &target, const Paint &current_color) {
        ll total_used = sol.use_amounts[0] + sol.use_amounts[1] + sol.use_amounts[2];

        ll current_use = max(0LL, current_color.count - sol.discard_count);
        ll sum         = total_used + current_use;

        if (sum <= 0) {
            sol.score = 1e9;
            return;
        }

        double r = 0, g = 0, b = 0;
        rep(i, 3) {
            r += colors[sol.color_ids[i]].red * sol.use_amounts[i];
            g += colors[sol.color_ids[i]].green * sol.use_amounts[i];
            b += colors[sol.color_ids[i]].blue * sol.use_amounts[i];
        }

        if (current_use > 0) {
            r += current_color.red * current_use;
            g += current_color.green * current_use;
            b += current_color.blue * current_use;
        }

        r /= sum;
        g /= sum;
        b /= sum;

        double dr = r - target.red;
        double dg = g - target.green;
        double db = b - target.blue;

        sol.score        = sqrt(dr * dr + dg * dg + db * db) * 10000 + total_used * d_penalty;
        sol.result_color = Paint(r, g, b, sum);
    }
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

    // 🤖 AI戦略決定
    MLStrategy strategy(k, h, t, d);

    // 🎨 最適色選択
    vector<int> selected = select_optimal_colors_ai(own_color, strategy.color_limit);
    vector<Paint> selected_colors(selected.size());
    rep(i, selected.size()) selected_colors[i] = own_color[selected[i]];

    // パレット初期化（シンプル版）
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    // 🧠 AIソルバー初期化
    SimulatedAnnealing sa_solver(strategy, selected_colors, selected, d);
    BeamSearch beam_solver(strategy, selected_colors, selected, d);

    ll c = (t / h) / 2;
    Paint current_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &target = target_color[hi];

        // 🔥 焼きなまし解
        Solution sa_solution = sa_solver.solve_single_target(target, current_color, c);

        // 🌟 ビームサーチ解
        Solution beam_solution = beam_solver.solve_target(target, current_color, c);

        // より良い解を選択
        Solution best_solution = (sa_solution.score < beam_solution.score) ? sa_solution : beam_solution;

        current_color = best_solution.result_color;

        // アクション出力
        rep(i, best_solution.discard_count) cout << 3 << spa << 0 << spa << 0 << el;
        rep(i, best_solution.use_amounts[0]) cout << 1 << spa << 0 << spa << 0 << spa
                                                  << selected[best_solution.color_ids[0]] << el;
        rep(i, best_solution.use_amounts[1]) cout << 1 << spa << 0 << spa << 0 << spa
                                                  << selected[best_solution.color_ids[1]] << el;
        rep(i, best_solution.use_amounts[2]) cout << 1 << spa << 0 << spa << 0 << spa
                                                  << selected[best_solution.color_ids[2]] << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        current_color.count--;
    }

    return 0;
}