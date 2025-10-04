#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

// template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
//    return os << m.val();
// }

template <class T> ostream &operator<<(ostream &os, const vector<T> &v) {
    os << '[';
    for (ll i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    return os << ']';
}
template <class T> ostream &operator<<(ostream &os, const set<T> &s) {
    os << '{';
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it != s.begin()) os << ", ";
        os << *it;
    }
    return os << '}';
}

template <class K, class V, class Comp, class Alloc>
ostream &operator<<(ostream &os, const map<K, V, Comp, Alloc> &m) {
    os << '{';
    for (auto &[k, v] : m) {
        if (k != m.begin()->first) os << ", ";
        os << k << ": " << v;
    }
    return os << '}';
}

// ----------------- デバッグ用 -----------------
namespace dbg {
template <typename... Ts> void impl(const char *names, Ts &&...xs) {
    string_view sv{names};
    array<string_view, sizeof...(Ts)> labels{};
    ll pos = 0, idx = 0;
    for (ll i = 0; i <= sv.size(); ++i) {
        if (i == sv.size() || sv[i] == ',') {
            ll start = pos;
            while (start < i && sv[start] == ' ') ++start; // 前方空白除去
            labels[idx++] = sv.substr(start, i - start);
            pos           = i + 1;
        }
    }
    ll id    = 0;
    auto out = [&](auto &&v) {
        cerr << labels[id++] << " = " << v;
        if (id < sizeof...(Ts)) cerr << ", ";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

// すべての通路が繋がっていて、かつ S→G が存在するか
bool all_open(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int N = (int)b.size();

    if (!inside(sx, sy, N, N) || !inside(gx, gy, N, N)) return false;
    if (b[sx][sy] == 'T' || b[gx][gy] == 'T') return false;

    int total = 0;
    rep(i, N) rep(j, N) if (b[i][j] == '.') total++;

    vector<vector<bool>> vis(N, vector<bool>(N, false));
    queue<pair<int, int>> q;
    vis[sx][sy] = true;
    q.push({sx, sy});
    int reached = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        reached++;

        rep(d, 4) {
            int nx = x + dx[d], ny = y + dy[d];

            if (!inside(nx, ny, N, N) || vis[nx][ny]) continue;
            if (b[nx][ny] == 'T') continue;
            vis[nx][ny] = true;
            q.push({nx, ny});
        }
    }
    return reached == total;
}

// ゴールの周辺に壁を置いて見えにくくする処理
void obscure_goal(vector<vector<char>> &b, int sx, int sy, int gx, int gy,
                  vector<pair<int, int>> &placed_out) {
    int N = b.size();

    vector<pair<int, int>> touched;

    // (x,y) に壁を置く（置けるなら）
    auto placeWall = [&](int x, int y) {
        if (!inside(x, y, N, N)) return;
        if ((x == sx && y == sy) || (x == gx && y == gy)) return;
        if (b[x][y] == '.') {
            b[x][y] = 'T';
            touched.emplace_back(x, y);
        }
    };

    auto rollback = [&]() {
        for (auto &p : touched) b[p.first][p.second] = '.';
        touched.clear();
    };

    auto dist_edge = [&](int x, int y) { return min(min(x, N - 1 - x), min(y, N - 1 - y)); };

    struct Cand {
        double score;
        vector<pair<int, int>> walls;
    };
    Cand best{
        -1e100,
        {},
    };

    rep(d, 4) { // 空きますの位置
        int nx = gx + dx[d], ny = gy + dy[d];
        if (!inside(nx, ny, N, N) || b[nx][ny] == 'T') continue; // 範囲外 or 壁

        rep(i, 2) { // 壁をうめる方向
            int nnx = nx + dy[d] * (i == 0 ? 1 : -1);
            int nny = ny - dx[d] * (i == 0 ? 1 : -1);

            touched.clear();
            rep(dd, 4) {
                if (dd == d) continue;
                int wx = gx + dx[dd], wy = gy + dy[dd];
                placeWall(wx, wy);
            }

            placeWall(nx + dx[d], ny + dy[d]); // 奥
            placeWall(nnx, nny);               // 横

            if (!all_open(b, sx, sy, gx, gy)) {
                rollback();
                continue;
            }

            int score = -dist_edge(nnx, nny);

            if (score > best.score) {
                best.score = score;
                best.walls = touched;
            }

            rollback();
        }
    }

    for (auto &[x, y] : best.walls) {
        if (b[x][y] == '.') b[x][y] = 'T';
        placed_out.emplace_back(x, y);
    }
}

// ==== ここから追記（または置換） ====

struct Eval {
    double score = -1e100;
};

struct SAParams {
    double TIME_LIMIT = 1.6;  // 秒（評価が重いためやや短縮）
    double T0         = 0.8;  // 初期温度
    double T1         = 0.02; // 終端温度
};

static inline uint64_t rng64() {
    static uint64_t x = 88172645463393265ull;
    x ^= x << 7;
    x ^= x >> 9;
    return x;
}
static inline int rand_int(int n) { return (int)(rng64() % n); }
static inline double rand01() { return (rng64() >> 11) * (1.0 / 9007199254740992.0); } // [0,1)

// SからBFS：到達距離/親/到達集合、S->G の最短路・曲がり回数も取る
static inline void bfs_from_s(const vector<vector<char>> &b, int sx, int sy, vector<int> &dist,
                              vector<int> &par) {
    int N          = (int)b.size();
    const int INFV = 1e9;
    dist.assign(N * N, INFV);
    par.assign(N * N, -1);
    auto id = [&](int x, int y) { return x * N + y; };
    queue<pair<int, int>> q;
    if (b[sx][sy] == '.') {
        dist[id(sx, sy)] = 0;
        q.push({sx, sy});
    }
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        int dcur = dist[id(x, y)];
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (!inside(nx, ny, N, N) || b[nx][ny] == 'T') continue;
            int nid = id(nx, ny);
            if (dist[nid] > dcur + 1) {
                dist[nid] = dcur + 1;
                par[nid]  = id(x, y); // 親
                q.push({nx, ny});
            }
        }
    }
}

static inline uint64_t calc_board_hash(const vector<vector<char>> &b) {
    uint64_t h = 1469598103934665603ull;
    for (const auto &row : b) {
        for (char c : row) {
            h ^= static_cast<unsigned char>(c);
            h *= 1099511628211ull;
        }
    }
    return h;
}

static inline int count_2x2(const vector<vector<char>> &b) {
    int N = (int)b.size();
    int c = 0;
    for (int i = 0; i + 1 < N; i++)
        for (int j = 0; j + 1 < N; j++)
            if (b[i][j] == '.' && b[i + 1][j] == '.' && b[i][j + 1] == '.' && b[i + 1][j + 1] == '.') c++;
    return c;
}

static inline int count_turns_on_path(const vector<int> &par, int N, int sx, int sy, int gx, int gy) {
    auto id = [&](int x, int y) { return x * N + y; };
    int g   = id(gx, gy);
    int s   = id(sx, sy);
    if (g != s && par[g] == -1) return 0;
    vector<pair<int, int>> path;
    for (int cur = g; cur != -1;) {
        int x = cur / N;
        int y = cur % N;
        path.emplace_back(x, y);
        if (cur == s) break;
        cur = par[cur];
    }
    if (path.empty() || path.back() != make_pair(sx, sy)) return 0;
    reverse(path.begin(), path.end());
    int turns = 0;
    for (int i = 1; i + 1 < (int)path.size(); i++) {
        int dx1 = path[i].first - path[i - 1].first;
        int dy1 = path[i].second - path[i - 1].second;
        int dx2 = path[i + 1].first - path[i].first;
        int dy2 = path[i + 1].second - path[i].second;
        if (dx1 != dx2 || dy1 != dy2) turns++;
    }
    return turns;
}

static inline void reach_graph_stats(const vector<vector<char>> &b, const vector<int> &dist, int sx,
                                     int sy, int &V, int &E2, int &loops, int &junctions) {
    int N   = (int)b.size();
    auto id = [&](int x, int y) { return x * N + y; };
    V = E2 = junctions = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] == '.' && dist[id(i, j)] < (int)1e9) {
                V++;
                int deg = 0;
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (inside(ni, nj, N, N) && b[ni][nj] == '.' && dist[id(ni, nj)] < (int)1e9) deg++;
                }
                E2 += deg;
                if (deg >= 3) junctions++;
            }
    int E = E2 / 2;
    if (V == 0) loops = 0;
    else loops = max(0, E - V + 1);
}

static inline int simulate_steps(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int N = (int)b.size();
    if (!inside(sx, sy, N, N) || !inside(gx, gy, N, N)) return -1;
    if (b[sx][sy] == 'T' || b[gx][gy] == 'T') return -1;

    const int total      = N * N;
    const int INF        = 1e9;
    const int MAX_STEPS  = N * N * 20;
    const array<int, 4> move_dx = {-1, 1, 0, 0};
    const array<int, 4> move_dy = {0, 0, -1, 1};

    vector<uint8_t> confirmed(total, 0);
    vector<int> dist_from_cur(total, INF);
    vector<int> dist_to_target(total, INF);
    vector<int> que(total);
    vector<int> candidates;
    candidates.reserve(total);

    auto id = [&](int x, int y) { return x * N + y; };

    auto is_blocked = [&](int x, int y) -> bool {
        if (!inside(x, y, N, N)) return true;
        int idx = id(x, y);
        if (!confirmed[idx]) return false;
        return b[x][y] == 'T';
    };

    auto bfs_provisional = [&](int sx0, int sy0, vector<int> &dist) {
        fill(dist.begin(), dist.end(), INF);
        if (is_blocked(sx0, sy0)) return;
        int start = id(sx0, sy0);
        int head  = 0;
        int tail  = 0;
        que[tail++] = start;
        dist[start] = 0;
        while (head < tail) {
            int v  = que[head++];
            int x0 = v / N;
            int y0 = v % N;
            int nd = dist[v] + 1;
            for (int d = 0; d < 4; d++) {
                int nx = x0 + (int)dx[d];
                int ny = y0 + (int)dy[d];
                if (is_blocked(nx, ny)) continue;
                int nid = id(nx, ny);
                if (dist[nid] <= nd) continue;
                dist[nid] = nd;
                que[tail++] = nid;
            }
        }
    };

    uint64_t seed = calc_board_hash(b);
    seed ^= ((uint64_t)(sx + 1) << 1) ^ ((uint64_t)(sy + 7) << 12);
    seed ^= ((uint64_t)(gx + 13) << 24) ^ ((uint64_t)(gy + 101) << 36);
    auto next_rand = [&]() -> uint64_t {
        seed ^= seed << 7;
        seed ^= seed >> 9;
        return seed;
    };
    auto pick_index = [&](int bound) { return (int)(next_rand() % bound); };

    int curx = sx, cury = sy;
    int target_x = -1, target_y = -1;
    bool has_target = false;
    confirmed[id(curx, cury)] = 1;

    int steps = 0;
    while (steps < MAX_STEPS) {
        if (curx == gx && cury == gy) return steps;

        for (int dir = 0; dir < 4; dir++) {
            int nx = curx;
            int ny = cury;
            while (true) {
                nx += move_dx[dir];
                ny += move_dy[dir];
                if (!inside(nx, ny, N, N)) break;
                int nid = id(nx, ny);
                if (!confirmed[nid]) confirmed[nid] = 1;
                if (b[nx][ny] == 'T') break;
            }
        }

        confirmed[id(curx, cury)] = 1;

        if (confirmed[id(gx, gy)]) {
            has_target = true;
            target_x   = gx;
            target_y   = gy;
        }

        bfs_provisional(curx, cury, dist_from_cur);

        if (has_target) {
            int tgt_id = id(target_x, target_y);
            if (dist_from_cur[tgt_id] >= INF) has_target = false;
        }

        if (!has_target || (confirmed[id(target_x, target_y)] && !(target_x == gx && target_y == gy))) {
            candidates.clear();
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    int idx = id(i, j);
                    if (dist_from_cur[idx] < INF && !confirmed[idx]) candidates.push_back(idx);
                }
            }
            if (candidates.empty()) return -1;
            int chosen = candidates[pick_index((int)candidates.size())];
            target_x   = chosen / N;
            target_y   = chosen % N;
            has_target = true;
        }

        bfs_provisional(target_x, target_y, dist_to_target);
        int cur_id = id(curx, cury);
        if (dist_to_target[cur_id] >= INF) return -1;

        int best_dir = -1;
        for (int dir = 0; dir < 4; dir++) {
            int nx = curx + move_dx[dir];
            int ny = cury + move_dy[dir];
            if (!inside(nx, ny, N, N)) continue;
            if (b[nx][ny] != '.') continue;
            int nid = id(nx, ny);
            if (dist_to_target[nid] < dist_to_target[cur_id]) {
                best_dir = dir;
                break;
            }
        }
        if (best_dir == -1) return -1;

        curx += move_dx[best_dir];
        cury += move_dy[best_dir];
        confirmed[id(curx, cury)] = 1;
        steps++;
    }

    return -1;
}

static inline Eval evaluate(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    Eval ev;

    vector<int> dist, par;
    int N = (int)b.size();
    bfs_from_s(b, sx, sy, dist, par);
    auto id = [&](int x, int y) { return x * N + y; };
    if (gx < 0 || gy < 0 || gx >= N || gy >= N) {
        ev.score = -1e95;
        return ev;
    }
    int dG = dist[id(gx, gy)];
    if (dG >= (int)1e9) {
        ev.score = -1e95;
        return ev;
    }

    int steps = simulate_steps(b, sx, sy, gx, gy);
    if (steps < 0) {
        ev.score = -1e95;
        return ev;
    }

    int two2 = count_2x2(b);

    int total_open = 0, reach_open = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] == '.') {
                total_open++;
                if (dist[id(i, j)] < (int)1e9) reach_open++;
            }
    int unreachable = total_open - reach_open;

    int V, E2, loops, junctions;
    reach_graph_stats(b, dist, sx, sy, V, E2, loops, junctions);
    int turns = count_turns_on_path(par, N, sx, sy, gx, gy);

    const double STEP_W      = 1.0;
    const double W_LEN       = 0.6;
    const double W_TURN      = 0.4;
    const double W_JUNC      = 0.3;
    const double W_LOOP      = 0.5;
    const double W_REACH     = 0.08;
    const double LOOP_CAP    = 200.0;
    const double PENALTY_2X2 = 800.0;
    const double PENALTY_UNR = 12.0;

    double score = 0.0;
    score += STEP_W * steps;
    score += W_LEN * dG;
    score += W_TURN * turns;
    score += W_JUNC * junctions;
    score += W_LOOP * min<double>(loops, LOOP_CAP);
    score += W_REACH * reach_open;
    score -= PENALTY_2X2 * two2;
    if (unreachable > 0) score -= PENALTY_UNR * sqrt((double)unreachable);

    ev.score = score;
    return ev;
}

// 2x2空白を S→G を壊さずに可能な限り即修正
static inline void greedy_fix_2x2(vector<vector<char>> &b, const vector<vector<char>> &orig, int sx,
                                  int sy, int gx, int gy) {
    int N         = (int)b.size();
    auto can_flip = [&](int x, int y) -> bool {
        if (!inside(x, y, N, N)) return false;
        if (orig[x][y] == 'T') return false; // 元から木は触らない
        // S, G は触らない
        // （G周囲は事前に obscure でピン留めされている前提）
        // S or G を潰さない
        return !((x == sx && y == sy) || (x == gx && y == gy));
    };

    // 試してダメなら戻す
    auto keeps_if_OK = [&](int x, int y) -> bool {
        char old = b[x][y];
        b[x][y]  = 'T';
        // S->G 到達性だけ確認
        vector<int> dist, par;
        bfs_from_s(b, sx, sy, dist, par);
        int N2  = N;
        auto id = [&](int a, int c) { return a * N2 + c; };
        bool ok = (b[sx][sy] == '.' && dist[id(gx, gy)] < (int)1e9);
        if (!ok) b[x][y] = old;
        return ok;
    };

    bool updated = true;
    int guard    = 0;
    while (updated && guard < N * N * 8) {
        updated = false;
        guard++;
        for (int i = 0; i + 1 < N; i++) {
            for (int j = 0; j + 1 < N; j++) {
                if (b[i][j] == '.' && b[i + 1][j] == '.' && b[i][j + 1] == '.' &&
                    b[i + 1][j + 1] == '.') {
                    // 4候補からランダム順で試す
                    array<pair<int, int>, 4> cands = {{{i, j}, {i + 1, j}, {i, j + 1}, {i + 1, j + 1}}};
                    // 少しランダム性
                    int r = rand_int(4);
                    swap(cands[0], cands[r]);
                    bool done = false;
                    for (auto [x, y] : cands) {
                        if (can_flip(x, y) && keeps_if_OK(x, y)) {
                            updated = true;
                            done    = true;
                            break;
                        }
                    }
                    // どれも置けない場合は一旦放置（SAに委ねる）
                }
            }
        }
    }
}

// 焼きなまし本体
void make_maze(vector<vector<char>> &b, int sx, int sy, int gx, int gy,
               const vector<vector<char>> &orig) {
    const SAParams P;
    int N = (int)b.size();

    // 変更可能セル（orig=='.' かつ S/G 以外）
    vector<pair<int, int>> mut;
    mut.reserve(N * N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (orig[i][j] == '.' && !(i == sx && j == sy) && !(i == gx && j == gy)) {
                mut.push_back({i, j});
            }
        }
    if (mut.empty()) return;

    // まず 2x2 の空白を手早く潰せるだけ潰す
    greedy_fix_2x2(b, orig, sx, sy, gx, gy);

    // 初期評価
    Eval cur                    = evaluate(b, sx, sy, gx, gy);
    vector<vector<char>> best_b = b;
    Eval best                   = cur;

    // 焼きなまし
    auto t0      = chrono::steady_clock::now();
    double TIMEL = P.TIME_LIMIT;
    int iter     = 0;

    while (true) {
        iter++;
        double t = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
        if (t > TIMEL) break;
        double ratio = t / TIMEL;
        double Temp  = P.T0 * pow(P.T1 / P.T0, ratio);

        // 1点フリップ（'.' <-> 'T'）ただし orig=='T' は触らない
        auto [x, y] = mut[rand_int((int)mut.size())];
        char old    = b[x][y];
        // トグル。ただし、origが'T'なら戻せないので mut に入っていない想定
        b[x][y] = (b[x][y] == '.' ? 'T' : '.');

        Eval nxt    = evaluate(b, sx, sy, gx, gy);
        double diff = nxt.score - cur.score;
        bool accept = (diff >= 0.0) || (exp(diff / Temp) > rand01());

        if (accept) {
            cur = nxt;
            if (nxt.score > best.score) {
                best   = nxt;
                best_b = b;
            }
        } else {
            // 戻す
            b[x][y] = old;
        }
    }

    // ベスト解に反映
    b = best_b;
}
// ==== ここまで追記 ====

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    // 入力の受け取りなど
    int N;
    int gx, gy;
    cin >> N >> gx >> gy;
    int sx = 0, sy = N / 2; // 入口

    vector b(N, vector<char>(N));
    rep(i, N) rep(j, N) cin >> b[i][j];

    vector origin = b;
    vector<pair<int, int>> pinned;

    obscure_goal(b, sx, sy, gx, gy, pinned);
    auto processed_origin = origin; // コピー
    for (auto &[x, y] : pinned) processed_origin[x][y] = 'T';

    make_maze(b, sx, sy, gx, gy, processed_origin);

    vector<pll> ans;

    rep(i, N) rep(j, N) {
        if (b[i][j] != origin[i][j]) ans.emplace_back(i, j);
    }

    bool first = true;

    while (true) {
        ll x, y;
        if (!(cin >> x >> y)) break;
        if (x == gx && y == gy) break;

        ll n;
        if (!(cin >> n)) break;
        vec xs(n), ys(n);
        rep(i, n) cin >> xs[i] >> ys[i];

        if (first) {
            first = false;
            cout << ans.size();
            for (auto &[x, y] : ans) cout << ' ' << x << ' ' << y;
            cout << endl;
        } else {
            cout << 0 << endl;
        }
    }
}
