#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x = -1;
    int y = -1;
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point &other) const { return !(*this == other); }
};

using Grid = vector<string>;

constexpr array<int, 4> DX{0, 1, 0, -1};
constexpr array<int, 4> DY{1, 0, -1, 0};

inline bool inside(int x, int y, int n) { return 0 <= x && x < n && 0 <= y && y < n; }

struct ConnectivityInfo {
    int reachable   = 0;
    int total_open  = 0;
    bool reach_goal = false;
};

ConnectivityInfo analyze_connectivity(const Grid &grid, Point start, Point goal) {
    const int n = static_cast<int>(grid.size());
    ConnectivityInfo info;
    if (!inside(start.x, start.y, n) || !inside(goal.x, goal.y, n)) return info;
    if (grid[start.x][start.y] == 'T' || grid[goal.x][goal.y] == 'T') return info;

    vector<vector<int>> visited(n, vector<int>(n, 0));
    queue<Point> q;
    q.push(start);
    visited[start.x][start.y] = 1;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == '.') info.total_open++;

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        info.reachable++;
        if (cur == goal) info.reach_goal = true;
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + DX[dir];
            int ny = cur.y + DY[dir];
            if (!inside(nx, ny, n)) continue;
            if (visited[nx][ny]) continue;
            if (grid[nx][ny] == 'T') continue;
            visited[nx][ny] = 1;
            q.push({nx, ny});
        }
    }
    return info;
}

bool layout_ok(const Grid &grid, Point start, Point goal) {
    ConnectivityInfo info = analyze_connectivity(grid, start, goal);
    return info.reach_goal && info.reachable == info.total_open;
}

vector<vector<int>> bfs_distances(const Grid &grid, Point start) {
    const int n   = static_cast<int>(grid.size());
    const int INF = 1e9;
    vector<vector<int>> dist(n, vector<int>(n, INF));
    if (!inside(start.x, start.y, n)) return dist;
    if (grid[start.x][start.y] == 'T') return dist;

    queue<Point> q;
    dist[start.x][start.y] = 0;
    q.push(start);

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + DX[dir];
            int ny = cur.y + DY[dir];
            if (!inside(nx, ny, n)) continue;
            if (grid[nx][ny] == 'T') continue;
            if (dist[nx][ny] <= dist[cur.x][cur.y] + 1) continue;
            dist[nx][ny] = dist[cur.x][cur.y] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}

vector<Point> reconstruct_path(Point start, Point goal, const vector<vector<int>> &dist) {
    const int n   = static_cast<int>(dist.size());
    const int INF = 1e9;
    vector<Point> path;
    if (!inside(goal.x, goal.y, n)) return path;
    if (dist[goal.x][goal.y] == INF) return path;
    Point cur = goal;
    path.push_back(goal);
    while (cur != start) {
        bool moved = false;
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + DX[dir];
            int ny = cur.y + DY[dir];
            if (!inside(nx, ny, n)) continue;
            if (dist[nx][ny] + 1 != dist[cur.x][cur.y]) continue;
            cur = {nx, ny};
            path.push_back(cur);
            moved = true;
            break;
        }
        if (!moved) return {};
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Point> shortest_path(const Grid &grid, Point start, Point goal) {
    vector<vector<int>> dist = bfs_distances(grid, start);
    return reconstruct_path(start, goal, dist);
}

void obscure_goal(Grid &grid, Point start, Point goal, vector<Point> &pinned_walls,
                  vector<Point> &reserved_open) {
    const int n   = static_cast<int>(grid.size());
    auto try_wall = [&](int x, int y) {
        if (!inside(x, y, n)) return;
        if (grid[x][y] != '.') return;
        char backup = grid[x][y];
        grid[x][y]  = 'T';
        if (layout_ok(grid, start, goal)) {
            pinned_walls.push_back({x, y});
        } else {
            grid[x][y] = backup;
        }
    };

    try_wall(goal.x + 1, goal.y + 1);
    try_wall(goal.x + 1, goal.y - 1);
    try_wall(goal.x - 1, goal.y + 1);
    try_wall(goal.x - 1, goal.y - 1);

    for (int dir = 0; dir < 4; ++dir) {
        int gate_x     = goal.x + DX[dir];
        int gate_y     = goal.y + DY[dir];
        int approach_x = gate_x + DX[dir];
        int approach_y = gate_y + DY[dir];
        if (inside(gate_x, gate_y, n) && grid[gate_x][gate_y] == '.')
            reserved_open.push_back({gate_x, gate_y});
        if (inside(approach_x, approach_y, n) && grid[approach_x][approach_y] == '.')
            reserved_open.push_back({approach_x, approach_y});
    }
}

uint64_t rng64() {
    static uint64_t x = 88172645463393265ull;
    x ^= x << 7;
    x ^= x >> 9;
    return x;
}

int rand_int(int n) { return static_cast<int>(rng64() % n); }

double rand_unit() { return (rng64() >> 11) * (1.0 / 9007199254740992.0); }

int count_open_2x2(const Grid &grid) {
    const int n = static_cast<int>(grid.size());
    int count   = 0;
    for (int i = 0; i + 1 < n; ++i) {
        for (int j = 0; j + 1 < n; ++j) {
            if (grid[i][j] == '.' && grid[i + 1][j] == '.' && grid[i][j + 1] == '.' &&
                grid[i + 1][j + 1] == '.')
                ++count;
        }
    }
    return count;
}

struct Eval {
    double score    = -1e100;
    int path_len    = -1;
    int turns       = 0;
    int junctions   = 0;
    int loops       = 0;
    int unreachable = 0;
    int open2x2     = 0;
};

vector<vector<int>> bfs_from_start(const Grid &grid, Point start) {
    const int n   = static_cast<int>(grid.size());
    const int INF = 1e9;
    vector<vector<int>> dist(n, vector<int>(n, INF));
    if (!inside(start.x, start.y, n) || grid[start.x][start.y] == 'T') return dist;

    queue<Point> q;
    dist[start.x][start.y] = 0;
    q.push(start);

    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + DX[dir];
            int ny = cur.y + DY[dir];
            if (!inside(nx, ny, n)) continue;
            if (grid[nx][ny] == 'T') continue;
            if (dist[nx][ny] <= dist[cur.x][cur.y] + 1) continue;
            dist[nx][ny] = dist[cur.x][cur.y] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}

int count_turns_on_path(const vector<Point> &path) {
    if (path.size() < 3) return 0;
    int turns = 0;
    for (size_t i = 1; i + 1 < path.size(); ++i) {
        Point a = path[i - 1];
        Point b = path[i];
        Point c = path[i + 1];
        int vx1 = b.x - a.x;
        int vy1 = b.y - a.y;
        int vx2 = c.x - b.x;
        int vy2 = c.y - b.y;
        if (vx1 != vx2 || vy1 != vy2) ++turns;
    }
    return turns;
}

Eval evaluate_grid(const Grid &grid, Point start, Point goal) {
    const int n = static_cast<int>(grid.size());
    Eval ev;
    ev.open2x2               = count_open_2x2(grid);
    vector<vector<int>> dist = bfs_from_start(grid, start);

    const int INF = 1e9;
    if (dist[goal.x][goal.y] == INF) {
        ev.score = -1e95 - 1000.0 * ev.open2x2;
        return ev;
    }

    ev.path_len = dist[goal.x][goal.y];

    int reachable_open = 0;
    int total_open     = 0;
    int loops          = 0;
    int junctions      = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != '.') continue;
            ++total_open;
            if (dist[i][j] == INF) continue;
            ++reachable_open;
            int deg = 0;
            for (int dir = 0; dir < 4; ++dir) {
                int ni = i + DX[dir];
                int nj = j + DY[dir];
                if (!inside(ni, nj, n)) continue;
                if (grid[ni][nj] == '.' && dist[ni][nj] != INF) ++deg;
            }
            if (deg >= 3) ++junctions;
            loops += deg;
        }
    }
    loops /= 2;
    if (reachable_open > 0) loops = max(0, loops - reachable_open + 1);

    vector<Point> path = reconstruct_path(start, goal, dist);
    ev.turns           = count_turns_on_path(path);
    ev.loops           = loops;
    ev.junctions       = junctions;
    ev.unreachable     = total_open - reachable_open;

    const double W_LEN               = 5.0;
    const double W_TURN              = 2.0;
    const double W_JUNC              = 1.2;
    const double W_LOOP              = 3.0;
    const double LOOP_CAP            = 200.0;
    const double PENALTY_2X2         = 1000.0;
    const double PENALTY_UNREACHABLE = 5.0;

    double score = 0.0;
    score += W_LEN * ev.path_len;
    score += W_TURN * ev.turns;
    score += W_JUNC * ev.junctions;
    score += W_LOOP * min<double>(ev.loops, LOOP_CAP);
    score -= PENALTY_2X2 * ev.open2x2;
    score -= PENALTY_UNREACHABLE * ev.unreachable;

    ev.score = score;
    return ev;
}

void fix_open_2x2(Grid &grid, const Grid &orig, Point start, Point goal) {
    const int n   = static_cast<int>(grid.size());
    auto can_flip = [&](int x, int y) {
        if (!inside(x, y, n)) return false;
        if (orig[x][y] == 'T') return false;
        if ((x == start.x && y == start.y) || (x == goal.x && y == goal.y)) return false;
        return true;
    };

    bool updated = true;
    int guard    = 0;
    while (updated && guard < n * n * 4) {
        updated = false;
        ++guard;
        for (int i = 0; i + 1 < n; ++i) {
            for (int j = 0; j + 1 < n; ++j) {
                if (!(grid[i][j] == '.' && grid[i + 1][j] == '.' && grid[i][j + 1] == '.' &&
                      grid[i + 1][j + 1] == '.'))
                    continue;
                array<Point, 4> cells{{{i, j}, {i + 1, j}, {i, j + 1}, {i + 1, j + 1}}};
                for (int t = 3; t > 0; --t) {
                    int k = rand_int(t + 1);
                    swap(cells[t], cells[k]);
                }
                for (auto cell : cells) {
                    if (!can_flip(cell.x, cell.y)) continue;
                    char backup          = grid[cell.x][cell.y];
                    grid[cell.x][cell.y] = 'T';
                    if (layout_ok(grid, start, goal)) {
                        updated = true;
                        break;
                    }
                    grid[cell.x][cell.y] = backup;
                }
            }
        }
    }
}

struct SAParams {
    double time_limit = 1.8;
    double t0         = 0.8;
    double t1         = 0.02;
};

void build_maze(Grid &grid, Point start, Point goal, const Grid &orig) {
    const int n = static_cast<int>(grid.size());
    vector<Point> candidates;
    candidates.reserve(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (orig[i][j] == 'T') continue;
            if ((i == start.x && j == start.y) || (i == goal.x && j == goal.y)) continue;
            candidates.push_back({i, j});
        }
    }
    if (candidates.empty()) return;

    fix_open_2x2(grid, orig, start, goal);

    SAParams params;
    auto best_grid    = grid;
    Eval current_eval = evaluate_grid(grid, start, goal);
    Eval best_eval    = current_eval;

    auto start_time = chrono::steady_clock::now();
    while (true) {
        double elapsed = chrono::duration<double>(chrono::steady_clock::now() - start_time).count();
        if (elapsed > params.time_limit) break;
        double progress    = elapsed / params.time_limit;
        double temperature = params.t0 * pow(params.t1 / params.t0, progress);

        Point target             = candidates[rand_int(static_cast<int>(candidates.size()))];
        char backup              = grid[target.x][target.y];
        grid[target.x][target.y] = (grid[target.x][target.y] == '.') ? 'T' : '.';
        Eval next_eval           = evaluate_grid(grid, start, goal);
        double diff              = next_eval.score - current_eval.score;
        bool accept              = diff >= 0.0 || exp(diff / temperature) > rand_unit();
        if (accept) {
            current_eval = next_eval;
            if (next_eval.score > best_eval.score) {
                best_eval = next_eval;
                best_grid = grid;
            }
        } else {
            grid[target.x][target.y] = backup;
        }
    }

    grid = best_grid;
}

void seal_unreachable(Grid &grid, Point start, Point goal) {
    const int n              = static_cast<int>(grid.size());
    vector<vector<int>> dist = bfs_from_start(grid, start);
    const int INF            = 1e9;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != '.') continue;
            if (dist[i][j] != INF) continue;
            if ((i == start.x && j == start.y) || (i == goal.x && j == goal.y)) continue;
            grid[i][j] = 'T';
        }
    }
}

void select_goal_gates(Grid &grid, Point start, Point goal, int open_limit) {
    const int n = static_cast<int>(grid.size());
    struct Candidate {
        Point gate;
        int dir;
        int path_len;
    };

    vector<Candidate> open_candidates;
    for (int dir = 0; dir < 4; ++dir) {
        Point gate{goal.x + DX[dir], goal.y + DY[dir]};
        if (!inside(gate.x, gate.y, n)) continue;
        if (grid[gate.x][gate.y] != '.') continue;
        open_candidates.push_back({gate, dir, 0});
    }

    if ((int)open_candidates.size() <= open_limit) return;

    const int INF = 1e9;
    // Evaluate path length when only this gate remains open
    for (size_t i = 0; i < open_candidates.size(); ++i) {
        for (size_t j = 0; j < open_candidates.size(); ++j) {
            if (j == i) continue;
            auto [gx, gy] = open_candidates[j].gate;
            grid[gx][gy] = 'T';
        }

        auto dist = bfs_from_start(grid, start);
        open_candidates[i].path_len = dist[goal.x][goal.y];

        for (size_t j = 0; j < open_candidates.size(); ++j) {
            if (j == i) continue;
            auto [gx, gy] = open_candidates[j].gate;
            grid[gx][gy] = '.';
        }
    }

    sort(open_candidates.begin(), open_candidates.end(), [&](const Candidate &a, const Candidate &b) {
        int la = (a.path_len >= INF) ? -1 : a.path_len;
        int lb = (b.path_len >= INF) ? -1 : b.path_len;
        if (la != lb) return la > lb; // prefer longer path
        return a.dir < b.dir;
    });

    array<bool, 4> keep{};
    int kept = 0;
    for (const auto &cand : open_candidates) {
        if (cand.path_len >= INF) continue;
        if (kept >= open_limit) break;
        keep[cand.dir] = true;
        ++kept;
    }

    if (kept == 0 && !open_candidates.empty()) {
        keep[open_candidates.front().dir] = true;
    }

    for (const auto &cand : open_candidates) {
        auto [gx, gy] = cand.gate;
        grid[gx][gy] = keep[cand.dir] ? '.' : 'T';
    }

    if (!layout_ok(grid, start, goal)) {
        for (const auto &cand : open_candidates) {
            if (keep[cand.dir]) continue;
            auto [gx, gy] = cand.gate;
            grid[gx][gy] = '.';
            keep[cand.dir] = true;
            if (layout_ok(grid, start, goal)) break;
        }
    }
}

struct GateInfo {
    Point gate;
    Point approach;
    bool blocked = false;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int gx, gy;
    if (!(cin >> n >> gx >> gy)) return 0;
    Point goal{gx, gy};
    Point start{0, n / 2};

    Grid grid(n);
    for (int i = 0; i < n; ++i) cin >> grid[i];

    Grid original = grid;
    vector<Point> pinned_walls;
    vector<Point> reserved_open;

    obscure_goal(grid, start, goal, pinned_walls, reserved_open);
    Grid processed_origin = original;
    for (auto cell : pinned_walls) processed_origin[cell.x][cell.y] = 'T';
    for (auto cell : reserved_open) processed_origin[cell.x][cell.y] = 'T';

    build_maze(grid, start, goal, processed_origin);
    seal_unreachable(grid, start, goal);
    select_goal_gates(grid, start, goal, 2);

    Grid current_board = grid;

    vector<Point> initial_moves;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] != original[i][j]) initial_moves.push_back({i, j});

    vector<GateInfo> gates;
    for (int dir = 0; dir < 4; ++dir) {
        Point gate{goal.x + DX[dir], goal.y + DY[dir]};
        Point approach{gate.x + DX[dir], gate.y + DY[dir]};
        if (!inside(gate.x, gate.y, n)) continue;
        if (!inside(approach.x, approach.y, n)) continue;
        if (current_board[gate.x][gate.y] != '.') continue;
        gates.push_back({gate, approach, false});
    }

    vector<vector<bool>> seen(n, vector<bool>(n, false));

    bool first_turn = true;
    while (true) {
        int ax, ay;
        if (!(cin >> ax >> ay)) break;
        Point current{ax, ay};
        if (current == goal) break;

        int m;
        if (!(cin >> m)) break;
        for (int i = 0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            if (inside(x, y, n)) seen[x][y] = true;
        }

        seen[current.x][current.y] = true;

        for (auto &gate_info : gates) {
            if (seen[gate_info.gate.x][gate_info.gate.y]) gate_info.blocked = true;
        }

        if (first_turn) {
            first_turn = false;
            cout << initial_moves.size();
            for (auto cell : initial_moves) cout << ' ' << cell.x << ' ' << cell.y;

            cout << '\n' << flush;
            continue;
        }

        vector<Point> placements;
        for (auto &gate_info : gates) {
            if (gate_info.blocked) continue;
            if (!(current == gate_info.approach)) continue;
            if (seen[gate_info.gate.x][gate_info.gate.y]) continue;
            if (current_board[gate_info.gate.x][gate_info.gate.y] != '.') continue;

            current_board[gate_info.gate.x][gate_info.gate.y] = 'T';
            if (layout_ok(current_board, start, goal)) {
                placements.push_back(gate_info.gate);
                gate_info.blocked = true;
                break;
            } else {
                current_board[gate_info.gate.x][gate_info.gate.y] = '.';
            }
        }


        cout << placements.size();
        for (auto cell : placements) cout << ' ' << cell.x << ' ' << cell.y;
        cout << '\n' << flush;
    }

    return 0;
}
