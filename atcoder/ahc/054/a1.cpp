#include <bits/stdc++.h>
using namespace std;

struct RNG {
    mt19937_64 eng;
    uniform_real_distribution<double> uni{0.0, 1.0};
    RNG(long long seed = -1) {
        if (seed < 0) seed = (long long)random_device{}();
        eng.seed(seed);
    }
    double r01() { return uni(eng); }
    int randint(int L, int R) {
        uniform_int_distribution<int> d(L, R);
        return d(eng);
    }
    bool coin() { return (eng() & 1ULL) != 0ULL; }
};

static const int DX[4] = {-1, 1, 0, 0};
static const int DY[4] = {0, 0, -1, 1};

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

struct EvaluRes {
    double score = 0.0;
    int pathLen = 0;
    int turns = 0;
    int choices = 0;
};

struct EvalHelper {
    const vector<string> &base;
    const vector<vector<int>> &used;
    int w, h;
    pair<int, int> S, G;

    EvaluRes evaluate(bool printOn = false) const {
        EvaluRes er;
        auto [sx, sy] = S;
        auto [gx, gy] = G;

        if (!inside(sx, sy, w, h) || !inside(gx, gy, w, h)) return er;
        if (base[sy][sx] == 'T' || base[gy][gx] == 'T') return er;
        if (!used[sy][sx] || !used[gy][gx]) return er;

        vector<vector<int>> dist(h, vector<int>(w, -1));
        vector<vector<pair<int, int>>> par(h, vector<pair<int, int>>(w, {-1, -1}));
        queue<pair<int, int>> q;
        dist[sy][sx] = 0;
        q.push({sx, sy});

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nx = x + DX[d], ny = y + DY[d];
                if (!inside(nx, ny, w, h)) continue;
                if (base[ny][nx] == 'T') continue;
                if (!used[ny][nx]) continue;
                if (dist[ny][nx] != -1) continue;
                dist[ny][nx] = dist[y][x] + 1;
                par[ny][nx]  = {x, y};
                q.push({nx, ny});
            }
        }

        if (dist[gy][gx] < 0) return er;

        vector<pair<int, int>> path;
        {
            int x = gx, y = gy;
            while (!(x == sx && y == sy)) {
                path.push_back({x, y});
                tie(x, y) = par[y][x];
            }
            path.push_back({sx, sy});
            reverse(path.begin(), path.end());
        }

        auto count_turns = [&]() {
            int t = 0;
            for (size_t i = 1; i + 1 < path.size(); ++i) {
                int x0 = path[i - 1].first, y0 = path[i - 1].second;
                int x1 = path[i].first, y1 = path[i].second;
                int x2 = path[i + 1].first, y2 = path[i + 1].second;
                int vx1 = x1 - x0, vy1 = y1 - y0;
                int vx2 = x2 - x1, vy2 = y2 - y1;
                if (vx1 != vx2 || vy1 != vy2) t++;
            }
            return t;
        };

        auto count_choices = [&]() {
            int c = 0;
            for (size_t i = 0; i < path.size(); ++i) {
                int x = path[i].first, y = path[i].second;
                int deg = 0;
                for (int d = 0; d < 4; ++d) {
                    int nx = x + DX[d], ny = y + DY[d];
                    if (!inside(nx, ny, w, h)) continue;
                    if (base[ny][nx] == 'T') continue;
                    if (!used[ny][nx]) continue;
                    deg++;
                }
                if (i == 0)
                    c += max(0, deg - 1);
                else
                    c += max(0, deg - 2);
            }
            return c;
        };

        auto side_branches = [&]() {
            vector<vector<char>> onPath(h, vector<char>(w, 0));
            for (auto &p : path) onPath[p.second][p.first] = 1;

            auto branch_len = [&](int sx, int sy, int px, int py) {
                int len = 0;
                int x = sx, y = sy;
                int px0 = px, py0 = py;
                while (true) {
                    len++;
                    int next_cnt = 0, nx = 0, ny = 0;
                    for (int d = 0; d < 4; ++d) {
                        int tx = x + DX[d], ty = y + DY[d];
                        if (!inside(tx, ty, w, h)) continue;
                        if (base[ty][tx] == 'T') continue;
                        if (!used[ty][tx]) continue;
                        if (tx == px0 && ty == py0) continue;
                        if (onPath[ty][tx]) continue;
                        next_cnt++;
                        nx = tx;
                        ny = ty;
                    }
                    if (next_cnt != 1) break;
                    px0 = x;
                    py0 = y;
                    x   = nx;
                    y   = ny;
                }
                return len;
            };

            vector<double> scores;
            for (size_t i = 0; i < path.size(); ++i) {
                int x = path[i].first, y = path[i].second;
                for (int d = 0; d < 4; ++d) {
                    int nx = x + DX[d], ny = y + DY[d];
                    if (!inside(nx, ny, w, h)) continue;
                    if (base[ny][nx] == 'T') continue;
                    if (!used[ny][nx]) continue;
                    bool isPrev = (i > 0 && nx == path[i - 1].first && ny == path[i - 1].second);
                    bool isNext =
                        (i + 1 < path.size() && nx == path[i + 1].first && ny == path[i + 1].second);
                    if (isPrev || isNext) continue;
                    int len = branch_len(nx, ny, x, y);
                    if (len > 0) {
                        double wgt = double(dist[gy][gx] - dist[y][x]) / double(max(1, dist[gy][gx]));
                        scores.push_back(wgt * double(len));
                    }
                }
            }
            sort(scores.begin(), scores.end(), greater<double>());
            int K = min({(int)scores.size(), (w + h) / 6, 8});
            double prod = 1.0;
            for (int i = 0; i < K; ++i) prod *= scores[i];
            return prod;
        };

        int pathLen = dist[gy][gx];
        int turns   = count_turns();
        int choices = count_choices();
        double side = side_branches();

        int manhattan = abs(gx - sx) + abs(gy - sy);
        double len_ratio = manhattan > 0 ? double(pathLen) / double(manhattan) : 1.0;

        auto squash = [](double v, double a1, double a2) {
            double t = v - a1;
            if (t <= 0.0) return 0.0;
            double s = t / (a2 - a1);
            return s / (s + 1.0);
        };

        double s_len  = squash(len_ratio, 1.1, 1.3);
        double s_turn = squash(double(turns) / max(1, pathLen), 0.10, 0.30);
        double s_cho  = squash(double(choices) / max(1, pathLen), 0.10, 0.25);

        er.score   = max(0.0, s_len) * max(0.0, s_turn) * max(0.0, s_cho) * max(1e-9, side);
        er.pathLen = pathLen;
        er.turns   = turns;
        er.choices = choices;

        if (printOn) {
            for (int _ = 0; _ < w + 2; ++_) cerr << "##";
            cerr << "\n";
            for (int y = 0; y < h; ++y) {
                cerr << "##";
                for (int x = 0; x < w; ++x) {
                    if (x == sx && y == sy)
                        cerr << "S ";
                    else if (x == gx && y == gy)
                        cerr << "G ";
                    else if (base[y][x] == 'T' || !used[y][x])
                        cerr << "##";
                    else
                        cerr << "  ";
                }
                cerr << "##\n";
            }
            for (int _ = 0; _ < w + 2; ++_) cerr << "##";
            cerr << "\n";
            cerr << "len " << pathLen << "  turns " << turns << "  choices " << choices << "\n";
            cerr << "len_ratio " << len_ratio << "  s_len " << s_len << "  s_turn " << s_turn
                 << "  s_cho " << s_cho << "\n";
            cerr << "side " << side << "  total " << er.score << "\n\n";
        }

        return er;
    }
};

static void clean_used(vector<vector<int>> &used, const vector<string> &base, pair<int, int> S) {
    int h = used.size();
    int w = used.empty() ? 0 : used[0].size();
    vector<vector<char>> seen(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    if (inside(S.first, S.second, w, h) && used[S.second][S.first]) {
        seen[S.second][S.first] = 1;
        q.push(S);
    }
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d], ny = y + DY[d];
            if (!inside(nx, ny, w, h)) continue;
            if (base[ny][nx] == 'T') continue;
            if (!used[ny][nx]) continue;
            if (seen[ny][nx]) continue;
            seen[ny][nx] = 1;
            q.push({nx, ny});
        }
    }
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            if (used[y][x] && !seen[y][x]) used[y][x] = 0;
}

static bool path_exists_used(const vector<string> &base, const vector<vector<int>> &used,
                             pair<int, int> S, pair<int, int> G) {
    int h = base.size();
    int w = base.empty() ? 0 : base[0].size();
    auto [sx, sy] = S;
    auto [gx, gy] = G;
    if (!inside(sx, sy, w, h) || !inside(gx, gy, w, h)) return false;
    if (base[sy][sx] == 'T' || base[gy][gx] == 'T') return false;
    if (!used[sy][sx] || !used[gy][gx]) return false;

    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    vis[sy][sx] = 1;
    q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) return true;
        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d], ny = y + DY[d];
            if (!inside(nx, ny, w, h)) continue;
            if (base[ny][nx] == 'T') continue;
            if (!used[ny][nx]) continue;
            if (vis[ny][nx]) continue;
            vis[ny][nx] = 1;
            q.push({nx, ny});
        }
    }
    return false;
}

static vector<pair<int, int>> shortest_path_base(const vector<string> &base, pair<int, int> S,
                                                 pair<int, int> G) {
    int h = base.size();
    int w = base.empty() ? 0 : base[0].size();
    auto [sx, sy] = S;
    auto [gx, gy] = G;
    if (!inside(sx, sy, w, h) || !inside(gx, gy, w, h)) return {};
    if (base[sy][sx] == 'T' || base[gy][gx] == 'T') return {};

    vector<vector<int>> dist(h, vector<int>(w, -1));
    vector<vector<pair<int, int>>> par(h, vector<pair<int, int>>(w, {-1, -1}));
    queue<pair<int, int>> q;
    dist[sy][sx] = 0;
    q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) break;
        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d], ny = y + DY[d];
            if (!inside(nx, ny, w, h)) continue;
            if (base[ny][nx] == 'T') continue;
            if (dist[ny][nx] != -1) continue;
            dist[ny][nx] = dist[y][x] + 1;
            par[ny][nx]  = {x, y};
            q.push({nx, ny});
        }
    }
    if (dist[gy][gx] == -1) return {};

    vector<pair<int, int>> path;
    int x = gx, y = gy;
    while (!(x == sx && y == sy)) {
        path.push_back({x, y});
        tie(x, y) = par[y][x];
    }
    path.push_back({sx, sy});
    reverse(path.begin(), path.end());
    return path;
}

vector<vector<int>> generate_maze_from_base_with_SG(const vector<string> &base, int trials = -1,
                                                    long long seed = -1, bool printBest = false) {
    int h = base.size();
    if (h == 0) return {};
    int w = base[0].size();

    pair<int, int> S{-1, -1}, G{-1, -1};
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            if (base[y][x] == 'S') S = {x, y};
            if (base[y][x] == 'G') G = {x, y};
        }
    if (S.first < 0 || G.first < 0) throw runtime_error("S and G must exist");

    if (trials < 0) trials = max(1, w * h * 3 / 2);
    RNG rng(seed);

    auto run_once = [&](vector<vector<int>> &used) {
        used.assign(h, vector<int>(w, 0));

        struct Trigger {
            int x, y;
            int px, py;
            int straight;
        };

        vector<Trigger> triggers;
        vector<char> inQueue(w * h, 0);
        vector<vector<int>> attempts(h, vector<int>(w, 0));

        auto inside_open = [&](int x, int y) {
            return inside(x, y, w, h) && base[y][x] != 'T';
        };

        auto push_trigger = [&](int x, int y, int px, int py, int straight) {
            if (!inside_open(x, y)) return;
            if (!used[y][x]) used[y][x] = 1;
            int idx = y * w + x;
            if (!inQueue[idx]) {
                triggers.push_back({x, y, px, py, straight});
                inQueue[idx] = 1;
            }
        };

        push_trigger(S.first, S.second, 0, 0, 0);

        auto sample_idx = [&]() {
            int sz = (int)triggers.size();
            if (sz <= 1) return 0;
            int pickCnt = min(5, sz);
            int bestIdx = -1;
            int bestKey = INT_MAX;
            for (int i = 0; i < pickCnt; ++i) {
                int idx = rng.randint(0, sz - 1);
                auto &t = triggers[idx];
                int noise = rng.randint(-(w + h) / 3, (w + h) / 3);
                int key   = t.x + t.y + noise;
                if (key < bestKey) {
                    bestKey = key;
                    bestIdx = idx;
                }
            }
            return bestIdx < 0 ? 0 : bestIdx;
        };

        int guard = 0;
        int guardLimit = max(400, w * h * 6);

        while (!triggers.empty() && guard++ < guardLimit) {
            int idx = sample_idx();
            Trigger cur = triggers[idx];
            int curIndex = cur.y * w + cur.x;
            triggers[idx] = triggers.back();
            triggers.pop_back();
            inQueue[curIndex] = 0;

            array<pair<int, int>, 4> dirs = {make_pair(-1, 0), make_pair(1, 0), make_pair(0, -1),
                                             make_pair(0, 1)};
            if (rng.coin()) swap(dirs[0], dirs[2]);

            bool hasUnopened = false;
            bool opened      = false;

            for (auto [dx, dy] : dirs) {
                int nx = cur.x + dx;
                int ny = cur.y + dy;
                if (!inside_open(nx, ny)) continue;
                if (used[ny][nx]) continue;
                hasUnopened = true;

                double open_p = 0.38;

                if (cur.px == dx && cur.py == dy)
                    open_p *= 1.0 / pow(3.0, double(max(1, cur.straight)));
                else
                    open_p *= 1.20;

                int distCur = abs(cur.x - G.first) + abs(cur.y - G.second);
                int distNxt = abs(nx - G.first) + abs(ny - G.second);
                if (distNxt < distCur)
                    open_p *= 0.75;
                else
                    open_p *= 1.05;

                int future = 0;
                for (int d = 0; d < 4; ++d) {
                    int fx = nx + DX[d], fy = ny + DY[d];
                    if (!inside_open(fx, fy)) continue;
                    if (used[fy][fx]) continue;
                    future++;
                }
                open_p *= (0.85 + 0.10 * double(future));
                open_p = min(0.9, max(0.05, open_p));

                if (rng.r01() < open_p) {
                    int straight = (cur.px == dx && cur.py == dy) ? (cur.straight + 1) : 1;
                    push_trigger(nx, ny, dx, dy, straight);
                    opened = true;
                }
            }

            if (!hasUnopened) continue;

            attempts[cur.y][cur.x]++;
            bool requeue = false;
            if (opened) {
                if (attempts[cur.y][cur.x] < 7) requeue = true;
            } else {
                if (attempts[cur.y][cur.x] < 3) requeue = true;
            }

            if (requeue) push_trigger(cur.x, cur.y, 0, 0, 0);
        }

        used[S.second][S.first] = 1;
        used[G.second][G.first] = 1;
        clean_used(used, base, S);
    };

    vector<vector<int>> bestUsed(h, vector<int>(w, 0));
    EvaluRes bestRes;
    bestRes.score   = -1.0;
    bestRes.pathLen = -1;

    for (int it = 0; it < trials; ++it) {
        vector<vector<int>> used;
        run_once(used);
        EvalHelper EH{base, used, w, h, S, G};
        auto er = EH.evaluate(false);
        if (er.score > bestRes.score ||
            (er.score == bestRes.score && er.pathLen > bestRes.pathLen) ||
            (er.score == bestRes.score && er.pathLen == bestRes.pathLen && er.turns < bestRes.turns) ||
            (er.score == bestRes.score && er.pathLen == bestRes.pathLen && er.turns == bestRes.turns &&
             er.choices > bestRes.choices)) {
            bestRes  = er;
            bestUsed = move(used);
        }
    }

    if (bestRes.pathLen <= 0) {
        bestUsed.assign(h, vector<int>(w, 0));
        vector<vector<int>> dist(h, vector<int>(w, -1));
        vector<vector<pair<int, int>>> par(h, vector<pair<int, int>>(w, {-1, -1}));
        queue<pair<int, int>> q;
        dist[S.second][S.first] = 0;
        q.push(S);
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (int d = 0; d < 4; ++d) {
                int nx = x + DX[d], ny = y + DY[d];
                if (!inside(nx, ny, w, h)) continue;
                if (base[ny][nx] == 'T') continue;
                if (dist[ny][nx] != -1) continue;
                dist[ny][nx] = dist[y][x] + 1;
                par[ny][nx]  = {x, y};
                q.push({nx, ny});
            }
        }
        if (dist[G.second][G.first] != -1) {
            int x = G.first, y = G.second;
            while (!(x == S.first && y == S.second)) {
                bestUsed[y][x] = 1;
                tie(x, y)      = par[y][x];
            }
            bestUsed[S.second][S.first] = 1;
        } else {
            bestUsed[S.second][S.first] = 1;
            bestUsed[G.second][G.first] = 1;
        }
    }

    clean_used(bestUsed, base, S);

    if (!path_exists_used(base, bestUsed, S, G)) {
        auto path = shortest_path_base(base, S, G);
        if (!path.empty()) {
            for (auto [x, y] : path) bestUsed[y][x] = 1;
            bestUsed[S.second][S.first] = 1;
            bestUsed[G.second][G.first] = 1;
            clean_used(bestUsed, base, S);
        }
    }

    if (!path_exists_used(base, bestUsed, S, G)) {
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (base[y][x] != 'T') bestUsed[y][x] = 1;
        bestUsed[S.second][S.first] = 1;
        bestUsed[G.second][G.first] = 1;
    }

    if (printBest) {
        EvalHelper EH{base, bestUsed, w, h, S, G};
        EH.evaluate(true);
    }

    return bestUsed;
}

vector<string> render_board(const vector<string> &base, const vector<vector<int>> &used) {
    int h = base.size();
    int w = base.empty() ? 0 : base[0].size();
    vector<string> out = base;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            if (base[y][x] == 'S' || base[y][x] == 'G') continue;
            out[y][x] = (used[y][x] ? '.' : 'T');
        }
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    int ti, tj;
    if (!(cin >> N >> ti >> tj)) return 0;

    vector<string> base(N);
    for (int i = 0; i < N; ++i) cin >> base[i];

    int si = 0;
    int sj = N / 2;
    base[sj][si] = 'S';
    base[tj][ti] = 'G';

    auto used = generate_maze_from_base_with_SG(base, N * N * 2, 123456789LL, false);

    vector<pair<int, int>> blocks;
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x) {
            if (base[y][x] == 'S' || base[y][x] == 'G') continue;
            if (base[y][x] == 'T') continue;
            if (!used[y][x]) blocks.emplace_back(y, x);
        }
        

    cout << blocks.size();
    for (auto [y, x] : blocks) cout << ' ' << y << ' ' << x;
    cout << '\n';
    cout.flush();

#ifdef LOCAL
    auto board = render_board(base, used);
    cerr << "Generated board:\n";
    for (auto &row : board) cerr << row << '\n';
#endif
    return 0;
}
