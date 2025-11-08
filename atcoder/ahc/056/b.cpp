#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pll = pair<ll, ll>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define all(x) (x).begin(), (x).end()
#define spa " "
#define el '\n'

struct Rule {
    int color_in;
    int state_in;
    int color_out;
    int state_out;
    char dir;
};

struct Solution {
    int C = 1;
    int Q = 1;
    vector<int> init_colors;
    vector<Rule> rules;
};

pair<int, int> choose_pair(int total) {
    total = max(1, total);
    int root = max(1, (int)sqrt((long double)total));
    int bestC = total, bestQ = total, bestSum = bestC + bestQ;
    for (int dc = 0; dc <= 1; ++dc) {
        for (int dq = 0; dq <= 1; ++dq) {
            int c = root + dc;
            int q = root + dq;
            if (c <= 0 || q <= 0) continue;
            if ((long long)c * q < total) continue;
            if (c + q < bestSum) {
                bestSum = c + q;
                bestC   = c;
                bestQ   = q;
            }
        }
    }
    return {bestC, bestQ};
}

vector<int> shortest_path(const vector<vector<int>> &g, int start, int goal) {
    const int N = g.size();
    vector<int> prev(N, -1);
    queue<int> que;
    que.push(start);
    prev[start] = start;
    while (!que.empty() && prev[goal] == -1) {
        int v = que.front();
        que.pop();
        for (int nv : g[v]) {
            if (prev[nv] != -1) continue;
            prev[nv] = v;
            que.push(nv);
        }
    }
    if (prev[goal] == -1) return {};
    vector<int> path;
    int cur = goal;
    while (cur != start) {
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(start);
    reverse(all(path));
    return path;
}

vector<int> build_route(const vector<vector<int>> &g, const vector<pll> &points, int n) {
    auto idx = [&](int x, int y) { return x * n + y; };
    vector<int> route;
    if (points.empty()) return route;
    route.push_back(idx(points[0].first, points[0].second));
    for (int i = 0; i + 1 < (int)points.size(); ++i) {
        int s = idx(points[i].first, points[i].second);
        int t = idx(points[i + 1].first, points[i + 1].second);
        auto path = shortest_path(g, s, t);
        if (path.empty()) continue;
        route.insert(route.end(), path.begin() + 1, path.end());
    }
    return route;
}

vector<int> compute_dirs(const vector<int> &route, int n) {
    auto get_dir = [&](int from, int to) -> int {
        int fx = from / n;
        int fy = from % n;
        int tx = to / n;
        int ty = to % n;
        if (fx == tx) {
            if (fy + 1 == ty) return 0; // R
            if (fy - 1 == ty) return 2; // L
        } else if (fy == ty) {
            if (fx + 1 == tx) return 1; // D
            if (fx - 1 == tx) return 3; // U
        }
        return -1;
    };
    vector<int> dirs;
    if (route.size() <= 1) return dirs;
    dirs.resize(route.size() - 1);
    for (int i = 0; i + 1 < (int)route.size(); ++i) dirs[i] = get_dir(route[i], route[i + 1]);
    return dirs;
}

vector<int> assign_labels(const vector<int> &route, const vector<int> &dirs,
                          const vector<int> &visit_cnt, int belt_max_len) {
    int nodes = route.size();
    vector<int> labels(nodes);
    iota(labels.begin(), labels.end(), 0);
    if ((int)dirs.size() <= 0) return labels;
    if (belt_max_len < 2) return labels;

    int steps = dirs.size();
    vector<char> used(steps, 0);
    for (int len = belt_max_len; len >= 2; --len) {
        for (int i = 0; i + len <= steps; ++i) {
            if (used[i]) continue;
            bool ok = true;
            int dir = dirs[i];
            for (int j = 0; j < len; ++j) {
                if (dirs[i + j] != dir) {
                    ok = false;
                    break;
                }
                if (visit_cnt[route[i + j]] != 1) {
                    ok = false;
                    break;
                }
                if (used[i + j]) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            int label_val = labels[i];
            for (int j = 0; j < len; ++j) {
                labels[i + j] = label_val;
                used[i + j]   = 1;
            }
        }
    }
    return labels;
}

optional<Solution> build_solution_with_belt(const vector<int> &route, int belt_max_len, int n) {
    const array<char, 4> DIR_CH = {'R', 'D', 'L', 'U'};
    int nodes                  = route.size();
    if (nodes == 0) {
        Solution sol;
        sol.C = sol.Q = 1;
        sol.init_colors.assign(n * n, 0);
        return sol;
    }
    int steps = nodes - 1;

    vector<int> dirs = compute_dirs(route, n);
    vector<int> visit_cnt(n * n, 0);
    for (int cell : route) ++visit_cnt[cell];

    vector<int> labels = assign_labels(route, dirs, visit_cnt, belt_max_len);
    unordered_map<int, int> comp;
    vector<int> comp_label(nodes);
    int next_id = 0;
    for (int i = 0; i < nodes; ++i) {
        int lab = labels[i];
        auto it = comp.find(lab);
        if (it == comp.end()) {
            comp[lab] = next_id;
            comp_label[i] = next_id++;
        } else {
            comp_label[i] = it->second;
        }
    }
    int stage_cnt     = max(1, next_id);
    auto [C, Q]       = choose_pair(stage_cnt);
    Solution sol;
    sol.C = C;
    sol.Q = Q;

    vector<int> init_color(n * n, -1);
    vector<queue<int>> stage_queue(n * n);
    for (int i = 0; i < nodes; ++i) {
        int cell = route[i];
        int lab  = comp_label[i];
        stage_queue[cell].push(lab);
        if (init_color[cell] == -1) init_color[cell] = lab % C;
    }
    for (int cell = 0; cell < n * n; ++cell) {
        if (init_color[cell] == -1) init_color[cell] = 0;
        stage_queue[cell].push(0);
        stage_queue[cell].pop();
    }
    sol.init_colors = init_color;

    vector<Rule> rules;
    rules.reserve(max(0, steps));
    unordered_map<long long, Rule> rule_map;

    auto encode = [&](int color, int state) -> long long {
        return (1LL * color << 32) ^ state;
    };

    for (int i = 0; i < steps; ++i) {
        int from = route[i];
        if (dirs[i] == -1) return nullopt;
        if (stage_queue[from].empty()) return nullopt;
        int next_cell_stage = stage_queue[from].front();
        stage_queue[from].pop();

        int lab        = comp_label[i];
        int next_lab   = comp_label[i + 1];
        int color_in   = lab % C;
        int state_in   = lab / C;
        int next_color = next_cell_stage % C;
        int next_state = next_lab / C;

        Rule new_rule{color_in, state_in, next_color, next_state, DIR_CH[dirs[i]]};
        long long key = encode(color_in, state_in);
        auto it       = rule_map.find(key);
        if (it == rule_map.end()) {
            rule_map[key] = new_rule;
            rules.push_back(new_rule);
        } else {
            const Rule &old = it->second;
            if (old.color_out != new_rule.color_out || old.state_out != new_rule.state_out || old.dir != new_rule.dir) {
                return nullopt; // conflicting behavior for same (color,state)
            }
        }
    }
    sol.rules = move(rules);
    return sol;
}

struct RuleEntry {
    bool exist      = false;
    int next_color  = 0;
    int next_state  = 0;
    char dir        = 'S';
};

bool can_move(int x, int y, char dir, const vector<string> &right_wall, const vector<string> &down_wall,
              int n, int &nx, int &ny) {
    nx = x;
    ny = y;
    if (dir == 'S') return true;
    if (dir == 'R') {
        if (y + 1 >= n) return false;
        if (right_wall[x][y] == '1') return false;
        ny = y + 1;
        return true;
    }
    if (dir == 'L') {
        if (y - 1 < 0) return false;
        if (right_wall[x][y - 1] == '1') return false;
        ny = y - 1;
        return true;
    }
    if (dir == 'D') {
        if (x + 1 >= n) return false;
        if (down_wall[x][y] == '1') return false;
        nx = x + 1;
        return true;
    }
    if (dir == 'U') {
        if (x - 1 < 0) return false;
        if (down_wall[x - 1][y] == '1') return false;
        nx = x - 1;
        return true;
    }
    return false;
}

bool simulate_solution(const Solution &sol, const vector<int> &route, const vector<pll> &points,
                       const vector<string> &right_wall, const vector<string> &down_wall, int n, ll T) {
    if (points.empty()) return true;
    ll steps = (ll)route.size() - 1;
    if (steps < 0) steps = 0;
    if (steps > T) return false;

    vector<int> board = sol.init_colors;
    vector<RuleEntry> table((size_t)sol.C * sol.Q);
    for (const auto &r : sol.rules) {
        ll idx = 1LL * r.color_in * sol.Q + r.state_in;
        if (idx < 0 || idx >= (ll)table.size()) return false;
        table[idx] = {true, r.color_out, r.state_out, r.dir};
    }

    auto idx_cell = [&](int x, int y) { return x * n + y; };
    int x = points[0].first;
    int y = points[0].second;
    int state = 0;
    int target_idx = 1;
    if ((int)points.size() == 1) return true;

    for (ll step = 0; step < steps; ++step) {
        int cell  = idx_cell(x, y);
        int color = board[cell];
        ll id     = 1LL * color * sol.Q + state;
        if (id < 0 || id >= (ll)table.size() || !table[id].exist) return false;
        auto rule = table[id];
        board[cell] = rule.next_color;
        state       = rule.next_state;
        int nx, ny;
        if (!can_move(x, y, rule.dir, right_wall, down_wall, n, nx, ny)) return false;
        x = nx;
        y = ny;
        if (target_idx < (int)points.size() && x == points[target_idx].first && y == points[target_idx].second)
            ++target_idx;
    }
    return target_idx == (int)points.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(20);

    int n, k;
    ll T;
    if (!(cin >> n >> k >> T)) return 0;

    vector<string> right_wall(n);
    rep(i, n) cin >> right_wall[i];
    vector<string> down_wall(max(0, n - 1));
    rep(i, n - 1) cin >> down_wall[i];

    vector<pll> points(k);
    rep(i, k) cin >> points[i].first >> points[i].second;

    auto idx = [&](int x, int y) { return x * n + y; };
    vector<vector<int>> g(n * n);
    rep(i, n) {
        rep(j, n - 1) {
            if (right_wall[i][j] == '1') continue;
            int a = idx(i, j);
            int b = idx(i, j + 1);
            g[a].push_back(b);
            g[b].push_back(a);
        }
    }
    rep(i, n - 1) {
        
        rep(j, n) {
            if (down_wall[i][j] == '1') continue;
            int a = idx(i, j);
            int b = idx(i + 1, j);
            g[a].push_back(b);
            g[b].push_back(a);
        }
    }

    vector<int> route = build_route(g, points, n);
    if (route.empty()) {
        Solution sol;
        sol.C = sol.Q = 1;
        sol.init_colors.assign(n * n, 0);
        cout << sol.C << spa << sol.Q << spa << 0 << el;
        rep(i, n) {
            rep(j, n) {
                if (j) cout << spa;
                cout << 0;
            }
            cout << el;
        }
        return 0;
    }

    const vector<int> BELT_OPTIONS = {3, 2, 1};
    bool found                     = false;
    Solution best_sol;
    int best_score = INT_MAX;

    for (int belt_len : BELT_OPTIONS) {
        auto sol_opt = build_solution_with_belt(route, belt_len, n);
        if (!sol_opt) continue;
        if (!simulate_solution(*sol_opt, route, points, right_wall, down_wall, n, T)) continue;
        int score = sol_opt->C + sol_opt->Q;
        if (!found || score < best_score) {
            found     = true;
            best_score = score;
            best_sol   = *sol_opt;
        }
    }

    if (!found) {
        auto fallback = build_solution_with_belt(route, 1, n);
        best_sol      = fallback.value();
    }

    cout << best_sol.C << spa << best_sol.Q << spa << best_sol.rules.size() << el;
    rep(i, n) {
        rep(j, n) {
            if (j) cout << spa;
            cout << best_sol.init_colors[idx(i, j)];
        }
        cout << el;
    }
    for (const auto &[c, s, nc, ns, d] : best_sol.rules) {
        cout << c << spa << s << spa << nc << spa << ns << spa << d << el;
    }
    return 0;
}
