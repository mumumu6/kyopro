#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pll = pair<ll, ll>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
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

struct CrossInfo {
    int n;
    int cx;
    int cy;
    vector<char> is_cross_cell;
    vector<char> is_gate_cell;
    vector<char> is_center_cell;
};

CrossInfo make_cross_info(int n) {
    CrossInfo info;
    info.n = n;
    info.cx = n / 2;
    info.cy = n / 2;
    info.is_cross_cell.assign(n * n, 0);
    info.is_gate_cell.assign(n * n, 0);
    info.is_center_cell.assign(n * n, 0);
    auto idx = [&](int x, int y) { return x * n + y; };
    for (int j = 0; j < n; ++j) info.is_cross_cell[idx(info.cx, j)] = 1;
    for (int i = 0; i < n; ++i) info.is_cross_cell[idx(i, info.cy)] = 1;
    info.is_center_cell[idx(info.cx, info.cy)] = 1;
    auto mark_gate = [&](int x, int y) {
        if (x < 0 || x >= n || y < 0 || y >= n) return;
        info.is_gate_cell[idx(x, y)] = 1;
    };
    int top_gate    = (0 + info.cx) / 2;
    int bottom_gate = (info.cx + (n - 1)) / 2;
    int left_gate   = (0 + info.cy) / 2;
    int right_gate  = (info.cy + (n - 1)) / 2;
    mark_gate(top_gate, info.cy);
    mark_gate(bottom_gate, info.cy);
    mark_gate(info.cx, left_gate);
    mark_gate(info.cx, right_gate);
    return info;
}

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



inline int dir_from_cells(int from, int to, int n) {
    int fx = from / n, fy = from % n;
    int tx = to / n, ty = to % n;
    if (fx == tx) {
        if (fy + 1 == ty) return 0;
        if (fy - 1 == ty) return 2;
    }
    if (fy == ty) {
        if (fx + 1 == tx) return 1;
        if (fx - 1 == tx) return 3;
    }
    return -1;
}

vector<int> shortest_path(const vector<vector<int>> &g, int start, int goal) {
    int N = g.size();
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
    reverse(path.begin(), path.end());
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

vector<int> weighted_shortest_path(const vector<vector<int>> &g, int start, int goal, const vector<int> &usage, int n,
                                   const CrossInfo &cross) {
    const ll INF = (ll)4e18;
    const ll STEP_COST = 1'000'000;
    const ll TURN_COST = 30'000;
    const ll REUSE_COST = 120'000;
    const ll WALKWAY_BONUS = 400'000;
    const ll NON_GATE_PENALTY = 3'000'000;
    auto encode = [&](int node, int dir) { return node * 5 + (dir + 1); };
    vector<ll> dist(g.size() * 5, INF);
    vector<int> prev(g.size() * 5, -1);
    using State = pair<ll, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    int start_idx = encode(start, -1);
    dist[start_idx] = 0;
    pq.emplace(0, start_idx);

    auto reuse_penalty = [&](int node) -> ll {
        int capped = min(usage[node], 8);
        if (capped <= 0) return 0;
        if (cross.is_cross_cell[node]) {
            return (REUSE_COST / 4) * min(capped, 2);
        }
        return REUSE_COST * capped;
    };
    auto is_walkway_edge = [&](int a, int b) -> bool {
        if (!cross.is_cross_cell[a] || !cross.is_cross_cell[b]) return false;
        int ax = a / n, ay = a % n;
        int bx = b / n, by = b % n;
        if (ax == bx && ax == cross.cx) return true;
        if (ay == by && ay == cross.cy) return true;
        return false;
    };

    int best_goal_idx = -1;
    while (!pq.empty()) {
        auto [d, idx] = pq.top();
        pq.pop();
        if (d != dist[idx]) continue;
        int v   = idx / 5;
        int dir = (idx % 5) - 1;
        if (v == goal) {
            best_goal_idx = idx;
            break;
        }
        for (int nv : g[v]) {
            int ndir = dir_from_cells(v, nv, n);
            if (ndir == -1) continue;
            ll w = STEP_COST + reuse_penalty(nv);
            if (is_walkway_edge(v, nv)) w = max<ll>(1000, w - WALKWAY_BONUS);
            bool entering = cross.is_cross_cell[nv] && !cross.is_cross_cell[v];
            bool exiting  = cross.is_cross_cell[v] && !cross.is_cross_cell[nv];
            if (entering && !cross.is_gate_cell[nv]) w += NON_GATE_PENALTY;
            if (exiting && !cross.is_gate_cell[v]) w += NON_GATE_PENALTY;
            if (dir != -1 && dir != ndir) w += TURN_COST;
            int nidx = encode(nv, ndir);
            if (dist[nidx] > d + w) {
                dist[nidx] = d + w;
                prev[nidx] = idx;
                pq.emplace(dist[nidx], nidx);
            }
        }
    }
    if (best_goal_idx == -1) return {};
    vector<int> path;
    int cur = best_goal_idx;
    while (cur != -1) {
        path.push_back(cur / 5);
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> build_route_with_bias(const vector<vector<int>> &g, const vector<pll> &points, int n, ll T,
                                  const CrossInfo &cross) {
    auto idx = [&](int x, int y) { return x * n + y; };
    vector<int> route;
    if (points.empty()) return route;
    vector<int> usage(n * n, 0);
    route.push_back(idx(points[0].first, points[0].second));
    usage[route.back()] = 1;
    for (int i = 0; i + 1 < (int)points.size(); ++i) {
        int s = idx(points[i].first, points[i].second);
        int t = idx(points[i + 1].first, points[i + 1].second);
        auto path = weighted_shortest_path(g, s, t, usage, n, cross);
        if (path.empty()) path = shortest_path(g, s, t);
        if (path.empty()) return {};
        for (int j = 1; j < (int)path.size(); ++j) {
            int cell = path[j];
            route.push_back(cell);
            if (usage[cell] < 1000) ++usage[cell];
        }
        if ((ll)route.size() - 1 > T) break;
    }
    if ((ll)route.size() - 1 > T) return {};
    return route;
}

struct Segment {
    vector<int> steps; // indices of steps (edges) included
    int dir;           // direction 0..3
};

struct Grouping {
    vector<int> group_id;
    vector<int> group_dir; // -1 for hub, otherwise direction
};

Grouping assign_groups(const vector<int> &step_dir, const vector<int> &segment_of_step,
                       const vector<int> &segment_pos, const vector<int> &segment_len, const vector<int> &selected,
                       const vector<char> &segment_is_walkway) {
    int steps = step_dir.size();
    Grouping res;
    res.group_id.assign(steps, -1);
    vector<int> dir_group_id(4, -1);
    vector<int> group_dir_list;
    auto new_group = [&](int dir_flag) {
        group_dir_list.push_back(dir_flag);
        return (int)group_dir_list.size() - 1;
    };

    for (int i = 0; i < steps; ++i) {
        bool corridor = false;
        int seg_id    = segment_of_step[i];
        bool walkway  = (seg_id != -1 && segment_is_walkway[seg_id]);
        if (seg_id != -1 && selected[seg_id] && walkway) {
            int pos = segment_pos[i];
            int len = segment_len[seg_id];
            if (pos > 0 && pos + 1 < len) corridor = true;
        }
        if (corridor) {
            int dir = step_dir[i];
            if (dir_group_id[dir] == -1) dir_group_id[dir] = new_group(dir);
            res.group_id[i] = dir_group_id[dir];
        } else {
            res.group_id[i] = new_group(-1);
        }
    }
    res.group_dir = move(group_dir_list);
    return res;
}

optional<Solution> build_solution_from_route(const vector<int> &route, const Grouping &grouping, int n) {
    const auto &group_id  = grouping.group_id;
    const auto &group_dir = grouping.group_dir;
    int steps             = (int)group_id.size();
    Solution sol;
    if (route.empty() || steps == 0) {
        sol.C = sol.Q = 1;
        sol.init_colors.assign(n * n, 0);
        return sol;
    }

    int stage_cnt = group_dir.size();
    auto [C, Q]   = choose_pair(stage_cnt);
    vector<int> stage_color(stage_cnt);
    vector<int> stage_state(stage_cnt);
    for (int i = 0; i < stage_cnt; ++i) {
        if (group_dir[i] != -1) {
            stage_color[i] = group_dir[i] % C;
            stage_state[i] = group_dir[i] % Q;
        } else {
            stage_color[i] = i % C;
            stage_state[i] = i / C;
        }
    }

    int N = n * n;
    vector<vector<int>> visits(N);
    for (int i = 0; i < steps; ++i) { visits[route[i]].push_back(group_id[i]); }

    vector<queue<int>> next_visit(N);
    vector<int> init_color(N, 0);
    for (int cell = 0; cell < N; ++cell) {
        auto &vec = visits[cell];
        if (!vec.empty()) init_color[cell] = stage_color[vec[0]];
        queue<int> q;
        for (int idx = 1; idx < (int)vec.size(); ++idx) q.push(vec[idx]);
        q.push(-1);
        next_visit[cell] = move(q);
    }

    auto encode = [&](int color, int state) -> long long { return (1LL * color << 32) ^ state; };
    unordered_map<long long, Rule> rule_map;
    vector<Rule> rules;
    rules.reserve(steps);

    for (int i = 0; i < steps; ++i) {
        int from     = route[i];
        int stage    = group_id[i];
        int color_in = stage_color[stage];
        int state_in = stage_state[stage];
        int future_stage = -1;
        if (!next_visit[from].empty()) {
            future_stage = next_visit[from].front();
            next_visit[from].pop();
        }
        bool corridor = (group_dir[stage] != -1);
        int next_color =
            corridor ? color_in : ((future_stage >= 0 && future_stage < stage_cnt) ? stage_color[future_stage] : 0);
        int next_state = (i + 1 < steps) ? stage_state[group_id[i + 1]] : 0;
        int dir_idx    = dir_from_cells(route[i], route[i + 1], n);
        if (dir_idx == -1) return nullopt;
        Rule new_rule{color_in, state_in, next_color, next_state, "RDLU"[dir_idx]};
        long long key = encode(color_in, state_in);
        auto it       = rule_map.find(key);
        if (it == rule_map.end()) {
            rule_map[key] = new_rule;
            rules.push_back(new_rule);
        } else {
            const Rule &old = it->second;
            if (old.color_out != new_rule.color_out || old.state_out != new_rule.state_out ||
                old.dir != new_rule.dir)
                return nullopt;
        }
    }

    sol.C           = C;
    sol.Q           = Q;
    sol.init_colors = move(init_color);
    sol.rules       = move(rules);
    return sol;
}

struct RuleEntry {
    bool exist      = false;
    int next_color  = 0;
    int next_state  = 0;
    char dir        = 'S';
};

bool step_move(int &x, int &y, char dir, int n, const vector<string> &right_wall, const vector<string> &down_wall) {
    if (dir == 'S') return true;
    if (dir == 'R') {
        if (y + 1 >= n) return false;
        if (right_wall[x][y] == '1') return false;
        y += 1;
        return true;
    }
    if (dir == 'L') {
        if (y - 1 < 0) return false;
        if (right_wall[x][y - 1] == '1') return false;
        y -= 1;
        return true;
    }
    if (dir == 'D') {
        if (x + 1 >= n) return false;
        if (down_wall[x][y] == '1') return false;
        x += 1;
        return true;
    }
    if (dir == 'U') {
        if (x - 1 < 0) return false;
        if (down_wall[x - 1][y] == '1') return false;
        x -= 1;
        return true;
    }
    return false;
}

bool simulate_solution(const Solution &sol, const vector<int> &route, const vector<pll> &points,
                       const vector<string> &right_wall, const vector<string> &down_wall, int n, ll T) {
    if (points.empty()) return true;
    if (route.size() <= 1) return true;
    int steps = (int)route.size() - 1;
    if (steps > T) return false;

    vector<int> board = sol.init_colors;
    vector<RuleEntry> table((size_t)sol.C * sol.Q);
    for (const auto &r : sol.rules) {
        ll idx = 1LL * r.color_in * sol.Q + r.state_in;
        if (idx < 0 || idx >= (ll)table.size()) return false;
        table[idx] = {true, r.color_out, r.state_out, r.dir};
    }

    auto idx = [&](int x, int y) { return x * n + y; };
    int x = points[0].first;
    int y = points[0].second;
    int state = 0;
    int target_idx = 1;
    if ((int)points.size() == 1) return true;

    for (int step = 0; step < steps; ++step) {
        int cell  = idx(x, y);
        int color = board[cell];
        ll id     = 1LL * color * sol.Q + state;
        if (id < 0 || id >= (ll)table.size()) return false;
        auto entry = table[id];
        if (!entry.exist) return false;
        board[cell] = entry.next_color;
        state       = entry.next_state;
        if (!step_move(x, y, entry.dir, n, right_wall, down_wall)) return false;
        if (target_idx < (int)points.size() && x == points[target_idx].first && y == points[target_idx].second)
            ++target_idx;
    }
    return target_idx = (int)points.size();
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

    CrossInfo cross = make_cross_info(n);
    vector<int> route = build_route_with_bias(g, points, n, T, cross);
    if (route.empty() || (ll)route.size() - 1 > T) route = build_route(g, points, n);
    if (route.size() <= 1) {
        cout << 1 << spa << 1 << spa << 0 << el;
        rep(i, n) {
            rep(j, n) {
                if (j) cout << spa;
                cout << 0;
            }
            cout << el;
        }
        return 0;
    }

    int steps = (int)route.size() - 1;
    vector<int> step_dir(steps);
    vector<char> step_walkway(steps, 0);
    auto is_walkway_edge = [&](int a, int b) -> bool {
        if (!cross.is_cross_cell[a] || !cross.is_cross_cell[b]) return false;
        int ax = a / n, ay = a % n;
        int bx = b / n, by = b % n;
        if (ax == bx && ax == cross.cx) return true;
        if (ay == by && ay == cross.cy) return true;
        return false;
    };
    for (int i = 0; i < steps; ++i) {
        step_dir[i]     = dir_from_cells(route[i], route[i + 1], n);
        step_walkway[i] = is_walkway_edge(route[i], route[i + 1]);
    }

    vector<int> visit_cnt(n * n, 0);
    for (int cell : route) ++visit_cnt[cell];

    vector<char> is_hub(route.size(), false);
    is_hub[0]     = true;
    is_hub.back() = true;
    auto is_walkway_cell = [&](int cell) { return cross.is_cross_cell[cell]; };
    for (int i = 0; i < (int)route.size(); ++i) {
        int cell = route[i];
        if (cross.is_center_cell[cell] || cross.is_gate_cell[cell]) {
            is_hub[i] = true;
            continue;
        }
        if (visit_cnt[cell] > 1 && !is_walkway_cell(cell)) is_hub[i] = true;
    }
    for (int i = 1; i + 1 < (int)route.size(); ++i)
        if (step_dir[i - 1] != step_dir[i]) is_hub[i] = true;

    vector<Segment> segments;
    vector<int> segment_of_step(steps, -1);
    for (int i = 0; i < steps; ++i) {
        if (is_hub[i] || is_hub[i + 1]) continue;
        int dir = step_dir[i];
        vector<int> list = {i};
        int j            = i + 1;
        while (j < steps && !is_hub[j] && !is_hub[j + 1] && step_dir[j] == dir) {
            list.push_back(j);
            ++j;
        }
        int seg_id = segments.size();
        for (int id : list) segment_of_step[id] = seg_id;
        segments.push_back({list, dir});
        i = j - 1;
    }

    int seg_cnt = segments.size();
    vector<int> segment_pos(steps, -1);
    vector<int> segment_len(seg_cnt, 0);
    for (int sid = 0; sid < seg_cnt; ++sid) {
        const auto &lst = segments[sid].steps;
        segment_len[sid] = lst.size();
        for (int idx = 0; idx < (int)lst.size(); ++idx) segment_pos[lst[idx]] = idx;
    }
    vector<char> segment_is_walkway(seg_cnt, 0);
    vector<int> selected(seg_cnt, 0);
    for (int sid = 0; sid < seg_cnt; ++sid) {
        const auto &lst = segments[sid].steps;
        bool walkway_segment = true;
        for (int step_idx : lst) {
            if (!step_walkway[step_idx]) {
                walkway_segment = false;
                break;
            }
        }
        if (walkway_segment) {
            segment_is_walkway[sid] = 1;
            selected[sid] = 1;
            continue;
        }
        bool clean      = true;
        for (int step_idx : lst) {
            if (visit_cnt[route[step_idx]] > 1 || visit_cnt[route[step_idx + 1]] > 1) {
                clean = false;
                break;
            }
        }
        if (((int)lst.size() >= 3 && clean) || (int)lst.size() >= 5) selected[sid] = 1;
    }

    auto evaluate = [&](const vector<int> &sel) -> optional<pair<Solution, int>> {
        Grouping grouping = assign_groups(step_dir, segment_of_step, segment_pos, segment_len, sel, segment_is_walkway);
        auto sol_opt      = build_solution_from_route(route, grouping, n);
        if (!sol_opt) return nullopt;
        if (!simulate_solution(*sol_opt, route, points, right_wall, down_wall, n, T)) return nullopt;
        int score = sol_opt->C + sol_opt->Q;
        return make_pair(move(*sol_opt), score);
    };

    auto base_eval = evaluate(selected);
    if (!base_eval) {
        cout << 1 << spa << 1 << spa << 0 << el;
        rep(i, n) {
            rep(j, n) {
                if (j) cout << spa;
                cout << 0;
            }
            cout << el;
        }
        return 0;
    }

    Solution best_sol = base_eval->first;
    int best_score    = base_eval->second;
    Solution cur_sol  = best_sol;
    int current_score = best_score;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    const double TIME_LIMIT = 1.2;
    auto time_start         = chrono::steady_clock::now();
    const double START_TEMP = 5.0;
    const double END_TEMP   = 0.2;

    auto elapsed = [&]() {
        return chrono::duration<double>(chrono::steady_clock::now() - time_start).count();
    };

    if (seg_cnt > 0) {
        uniform_real_distribution<double> uni01(0.0, 1.0);
        while (elapsed() < TIME_LIMIT) {
            int id = rng() % seg_cnt;
            selected[id] ^= 1;
            auto eval = evaluate(selected);
            if (!eval) {
                selected[id] ^= 1;
                continue;
            }
            double progress = elapsed() / TIME_LIMIT;
            double temp     = START_TEMP + (END_TEMP - START_TEMP) * progress;
            int new_score   = eval->second;
            double delta    = new_score - current_score;
            if (delta < 0 || uni01(rng) < exp(-delta / temp)) {
                current_score = new_score;
                cur_sol       = eval->first;
                if (new_score < best_score) {
                    best_score = new_score;
                    best_sol   = cur_sol;
                }
            } else {
                selected[id] ^= 1;
            }
        }
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
