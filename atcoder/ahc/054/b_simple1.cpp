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

// 方向ベクトルは既存の dx, dy を使用（x=行, y=列）
static inline bool is_open(const vector<vector<char>> &b, int x, int y) {
    int N = (int)b.size();
    return inside(x, y, N, N) && b[x][y] == '.';
}

struct Eval {
    double score    = -1e100;
    int path_len    = -1;
    int turns       = 0;
    int junctions   = 0;
    int loops       = 0;
    int two2        = 0;
    int unreachable = 0;
};

struct SAParams {
    double TIME_LIMIT = 1.8;  // 秒
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

// ==== NEW: 2x2 空白と 追加T4 の検出・掃除 ====
static inline int count_2x2(const vector<vector<char>> &b) {
    int N = (int)b.size(), c = 0;
    for (int i = 0; i + 1 < N; i++)
        for (int j = 0; j + 1 < N; j++)
            if (b[i][j] == '.' && b[i + 1][j] == '.' && b[i][j + 1] == '.' && b[i + 1][j + 1] == '.')
                c++;
    return c;
}
static inline bool is_T4(const vector<vector<char>> &b, int x, int y) {
    int N = (int)b.size(), cnt = 0;
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (!inside(nx, ny, N, N) || b[nx][ny] == 'T') cnt++;
    }
    return cnt == 4;
}
static inline int count_added_T4(const vector<vector<char>> &b, const vector<vector<char>> &orig) {
    int N = (int)b.size(), c = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] == 'T' && orig[i][j] == '.' && is_T4(b, i, j)) c++;
    return c;
}
static inline void cleanup_added_T4(vector<vector<char>> &b, const vector<vector<char>> &orig) {
    int N = (int)b.size();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (b[i][j] == 'T' && orig[i][j] == '.' && is_T4(b, i, j)) b[i][j] = '.';
}

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

// 最短路の曲がり回数
static inline int count_turns_on_path(const vector<int> &par, int N, int sx, int sy, int gx, int gy) {
    auto id = [&](int x, int y) { return x * N + y; };
    int g = id(gx, gy), s = id(sx, sy);
    if (par[g] == -1 && !(gx == sx && gy == sy)) return 0;
    vector<pair<int, int>> path;
    int cur = g;
    if (!(gx == sx && gy == sy)) {
        while (cur != -1) {
            int x = cur / N, y = cur % N;
            path.push_back({x, y});
            if (cur == s) break;
            cur = par[cur];
        }
        if (path.empty() || path.back() != make_pair(sx, sy)) return 0;
        reverse(path.begin(), path.end());
    } else {
        path.push_back({sx, sy});
    }

    int turns = 0;
    for (int k = 1; k + 1 < (int)path.size(); k++) {
        int x0 = path[k - 1].first, y0 = path[k - 1].second;
        int x1 = path[k].first, y1 = path[k].second;
        int x2 = path[k + 1].first, y2 = path[k + 1].second;
        int dx1 = x1 - x0, dy1 = y1 - y0;
        int dx2 = x2 - x1, dy2 = y2 - y1;
        if (dx1 != dx2 || dy1 != dy2) turns++;
    }
    return turns;
}

// 到達集合上での E, V, ループ数と分岐数（deg>=3）
static inline void reach_graph_stats(const vector<vector<char>> &b, const vector<int> &dist, int sx,
                                     int sy, int &V, int &E2, int &loops, int &junctions) {
    int N     = (int)b.size();
    auto id   = [&](int x, int y) { return x * N + y; };
    V         = 0;
    E2        = 0;
    junctions = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (b[i][j] == '.' && dist[id(i, j)] < (int)1e9) {
                V++;
                int deg = 0;
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (inside(ni, nj, N, N) && b[ni][nj] == '.' && dist[id(ni, nj)] < (int)1e9) deg++;
                }
                E2 += deg; // 後で /2
                if (deg >= 3) junctions++;
            }
        }
    }
    int E = E2 / 2;
    // 到達成分は1個想定（到達集合なので）かつ S が開いていれば連結成分=1
    // ただし S が壁なら V=0
    if (V == 0) loops = 0;
    else loops = max(0, E - V + 1);
}

struct ConnectivityInfo {
    bool valid = false;
    int loops  = 0;
};

static inline ConnectivityInfo analyze_connectivity(const vector<vector<char>> &b, int sx, int sy,
                                                    int gx, int gy) {
    ConnectivityInfo info;
    const int N = (int)b.size();
    if (!inside(sx, sy, N, N) || !inside(gx, gy, N, N)) return info;
    if (b[sx][sy] != '.' || b[gx][gy] != '.') return info;

    auto id = [&](int x, int y) { return x * N + y; };
    vector<int> dist(N * N, -1);
    queue<pair<int, int>> q;
    dist[id(sx, sy)] = 0;
    q.emplace(sx, sy);
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (!inside(nx, ny, N, N)) continue;
            if (b[nx][ny] == 'T') continue;
            int nid = id(nx, ny);
            if (dist[nid] != -1) continue;
            dist[nid] = dist[id(x, y)] + 1;
            q.emplace(nx, ny);
        }
    }

    bool goal_ok = dist[id(gx, gy)] != -1;
    int total_open = 0;
    int V          = 0;
    long long E2   = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (b[i][j] != '.') continue;
            ++total_open;
            int vid = id(i, j);
            if (dist[vid] == -1) continue;
            ++V;
            for (int d = 0; d < 4; ++d) {
                int ni = i + dx[d], nj = j + dy[d];
                if (!inside(ni, nj, N, N)) continue;
                if (b[ni][nj] != '.') continue;
                if (dist[id(ni, nj)] == -1) continue;
                ++E2;
            }
        }
    }

    bool all_reached = (V == total_open);
    info.valid        = goal_ok && all_reached;
    if (info.valid && V > 0) {
        int E = (int)(E2 / 2);
        info.loops = max(0, E - V + 1);
    }
    return info;
}

static inline Eval evaluate(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int N = (int)b.size();
    Eval ev;
    // 2x2
    ev.two2 = count_2x2(b);

    // BFS
    vector<int> dist, par;
    bfs_from_s(b, sx, sy, dist, par);
    auto id = [&](int x, int y) { return x * N + y; };
    int dG  = dist[id(gx, gy)];
    if (dG >= (int)1e9) {
        // S→G不達は超大罰
        ev.score       = -1e95 - 1e3 * ev.two2;
        ev.path_len    = -1;
        ev.unreachable = 0;
        return ev;
    }
    ev.path_len = dG;

    // 到達不能空きマス
    int total_open = 0, reach_open = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (b[i][j] == '.') {
                total_open++;
                if (dist[id(i, j)] < (int)1e9) reach_open++;
            }
        }
    ev.unreachable = total_open - reach_open;

    // ループ・分岐
    int V, E2, loops, junc;
    reach_graph_stats(b, dist, sx, sy, V, E2, loops, junc);
    ev.loops     = loops;
    ev.junctions = junc;

    // 最短路の曲がり
    ev.turns = count_turns_on_path(par, N, sx, sy, gx, gy);

    // ---- スコア合成 ----
    // 重みは適宜調整可。2x2は強罰、S->G長さ＆曲がり＆分岐＆ループを報酬、到達不能は軽い罰
    const double W_LEN    = 5.0;
    const double W_TURN   = 2.0;
    const double W_JUNC   = 1.2;
    const double W_LOOP   = 3.0;    // ループは程々に
    const double LOOP_CAP = 200.0;  // 飽和上限
    const double P_2X2    = 1000.0; // 2x2 は絶対潰す
    const double P_UNR    = 5.0;    // 到達不能はやや罰

    double s = 0.0;
    s += W_LEN * ev.path_len;
    s += W_TURN * ev.turns;
    s += W_JUNC * ev.junctions;
    s += W_LOOP * min<double>(ev.loops, LOOP_CAP);
    s -= P_2X2 * ev.two2;
    s -= P_UNR * ev.unreachable;

    ev.score = s;
    return ev;
}

// 2x2空白を S→G を壊さずに可能な限り即修正
static inline void greedy_fix_2x2(vector<vector<char>> &b, const vector<vector<char>> &orig, int sx,
                                  int sy, int gx, int gy) {
    int N         = (int)b.size();
    auto can_flip = [&](int x, int y) -> bool {
        if (!inside(x, y, N, N)) return false;
        if (orig[x][y] == 'T') return false;                    // 元から木は触らない
        return !((x == sx && y == sy) || (x == gx && y == gy)); // S/Gは触らない
    };

    // ★ ここを all_open に差し替え（非連結を作らない）
    auto keeps_if_OK = [&](int x, int y) -> bool {
        char old = b[x][y];
        b[x][y]  = 'T';
        bool ok  = analyze_connectivity(b, sx, sy, gx, gy).valid;
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
                    array<pair<int, int>, 4> cands = {{{i, j}, {i + 1, j}, {i, j + 1}, {i + 1, j + 1}}};
                    int r                          = rand_int(4);
                    swap(cands[0], cands[r]);
                    for (auto [x, y] : cands) {
                        if (can_flip(x, y) && keeps_if_OK(x, y)) {
                            updated = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

// 非連結を局所的に接続する“安全な”修復（1マス or 直線2マス）

// 既存の bfs_from_s と reach_graph_stats を使ってループ数を返す
static inline int loops_all_open_connected(const vector<vector<char>> &b, int sx, int sy) {
    int N = (int)b.size();
    vector<int> dist, par;
    bfs_from_s(b, sx, sy, dist, par);
    int V, E2, loops, junc;
    reach_graph_stats(b, dist, sx, sy, V, E2, loops, junc);
    return loops; // 連結なら loops = E - V + 1
}

// 成功すれば true。連結のまま終了 or 進展なしで false。
static bool repair_connectivity_local(vector<vector<char>> &b, const vector<vector<char>> &orig, int sx,
                                      int sy, int gx, int gy, int LOOPS_MIN = 1) {
    const int N  = (int)b.size();
    auto is_mutT = [&](int x, int y) {
        return inside(x, y, N, N) && orig[x][y] == '.' && b[x][y] == 'T';
    };

    auto ok_board = [&]() {
        if (count_2x2(b) > 0) return false;
        ConnectivityInfo ci = analyze_connectivity(b, sx, sy, gx, gy);
        if (!ci.valid) return false;
        return ci.loops >= LOOPS_MIN;
    };

    // Sから'.'だけで到達可否を取る
    auto mark_reach = [&]() {
        vector<int> dist, par;
        bfs_from_s(b, sx, sy, dist, par);
        vector<vector<uint8_t>> vis(N, vector<uint8_t>(N, 0));
        auto id = [&](int x, int y) { return x * N + y; };
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (b[i][j] == '.' && dist[id(i, j)] < (int)1e9) vis[i][j] = 1;
        return vis;
    };

    int guard = N * N * 4;
    while (guard--) {
        auto vis       = mark_reach();
        bool connected = true;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                if (b[i][j] == '.' && !vis[i][j]) {
                    connected = false;
                    break;
                }
        if (connected) return true;

        bool progressed = false;

        // 1) 1マス開けで接続できる可変壁を探す
        for (int i = 0; i < N && !progressed; i++) {
            for (int j = 0; j < N && !progressed; j++) {
                if (!is_mutT(i, j)) continue;
                bool adjR = false, adjU = false;
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (!inside(ni, nj, N, N) || b[ni][nj] == 'T') continue;
                    if (b[ni][nj] == '.' && vis[ni][nj]) adjR = true;
                    if (b[ni][nj] == '.' && !vis[ni][nj]) adjU = true;
                }
                if (adjR && adjU) {
                    char old = b[i][j];
                    b[i][j]  = '.';
                    if (ok_board()) {
                        progressed = true;
                        break;
                    }
                    b[i][j] = old;
                }
            }
        }
        if (progressed) continue;

        // 2) 直線2マスのトンネル（i,j と i+dx, j+dy を同時に開ける）
        for (int i = 0; i < N && !progressed; i++) {
            for (int j = 0; j < N && !progressed; j++) {
                if (!is_mutT(i, j)) continue;
                for (int d = 0; d < 4 && !progressed; d++) {
                    int k = i + dx[d], l = j + dy[d];
                    if (!is_mutT(k, l)) continue;

                    // 端点が R と U にそれぞれ隣接していることを確認
                    auto hasAdj = [&](int x, int y, bool wantR) {
                        for (int t = 0; t < 4; t++) {
                            int nx = x + dx[t], ny = y + dy[t];
                            if (!inside(nx, ny, N, N) || b[nx][ny] == 'T') continue;
                            if (b[nx][ny] == '.' && (bool)vis[nx][ny] == wantR) return true;
                        }
                        return false;
                    };
                    bool okEnds = (hasAdj(i, j, true) && hasAdj(k, l, false)) ||
                                  (hasAdj(i, j, false) && hasAdj(k, l, true));
                    if (!okEnds) continue;

                    char a = b[i][j], c = b[k][l];
                    b[i][j] = '.';
                    b[k][l] = '.';
                    if (ok_board()) {
                        progressed = true;
                        break;
                    }
                    b[i][j] = a;
                    b[k][l] = c;
                }
            }
        }

        if (!progressed) return false; // これ以上の局所接続が見つからない
    }
    return false;
}

// ==== NEW: Adventurer simulator (faithful to statement) ====

struct XRng {
    uint64_t x;
    XRng(uint64_t seed = 88172645463393265ull) : x(seed) {}
    inline uint64_t next() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    inline int randint(int n) { return (int)(next() % n); }
};

// 上下左右の優先順（問題文準拠）
static const int UX[4] = {-1, 1, 0, 0};
static const int UY[4] = {0, 0, -1, 1};

struct SimResult {
    int turns;
    bool reached;
};

// 高速シミュレーション：距離計算を必要時のみ更新し、探索を再利用する
static SimResult simulate_adventurer(const vector<vector<char>> &b, int sx, int sy, int gx, int gy,
                                     uint64_t seed = 123456789ull, int hard_cap_mul = 10) {
    const int N  = (int)b.size();
    const int NN = N * N;
    const int INF = 1e9;
    XRng rng(seed);

    static vector<uint8_t> confirmed;
    static vector<int> dist;
    static vector<int> q;
    static vector<int> visited;
    static vector<int> unconf;
    static int allocated = 0;

    if (allocated != NN) {
        allocated = NN;
        confirmed.assign(NN, 0);
        dist.assign(NN, INF);
        q.resize(NN);
        visited.assign(NN, 0);
        unconf.reserve(NN);
    } else {
        fill(confirmed.begin(), confirmed.end(), 0);
    }

    auto id = [&](int x, int y) { return x * N + y; };
    auto insideN = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };

    int goal_id = id(gx, gy);

    auto can_walk_provisional = [&](int v) -> bool {
        int x = v / N, y = v % N;
        return !(confirmed[v] && b[x][y] == 'T');
    };

    bool dist_valid = false;
    int stamp_counter = 1;

    auto bfs_reachable = [&](int start) {
        unconf.clear();
        int stamp = ++stamp_counter;
        if (stamp_counter == INT_MAX) {
            fill(visited.begin(), visited.end(), 0);
            stamp_counter = 2;
            stamp         = ++stamp_counter;
        }
        if (!can_walk_provisional(start)) return stamp;
        int head = 0, tail = 0;
        q[tail++] = start;
        visited[start] = stamp;
        while (head < tail) {
            int v = q[head++];
            if (!confirmed[v]) unconf.push_back(v);
            int x = v / N, y = v % N;
            for (int dir = 0; dir < 4; ++dir) {
                int nx = x + UX[dir], ny = y + UY[dir];
                if (!insideN(nx, ny)) continue;
                int nv = id(nx, ny);
                if (!can_walk_provisional(nv)) continue;
                if (visited[nv] == stamp) continue;
                visited[nv] = stamp;
                q[tail++]  = nv;
            }
        }
        return stamp;
    };

    auto recompute_dist = [&](int target, int cur_id) -> bool {
        if (!can_walk_provisional(target)) return false;
        fill(dist.begin(), dist.end(), INF);
        int head = 0, tail = 0;
        q[tail++] = target;
        dist[target] = 0;
        while (head < tail) {
            int v = q[head++];
            int x = v / N, y = v % N;
            int nd = dist[v] + 1;
            for (int dir = 0; dir < 4; ++dir) {
                int nx = x + UX[dir], ny = y + UY[dir];
                if (!insideN(nx, ny)) continue;
                int nv = id(nx, ny);
                if (!can_walk_provisional(nv)) continue;
                if (dist[nv] <= nd) continue;
                dist[nv] = nd;
                q[tail++] = nv;
            }
        }
        return dist[cur_id] != INF;
    };

    auto reveal_from = [&](int cx, int cy) -> bool {
        int cur_id = id(cx, cy);
        confirmed[cur_id] = 1;
        bool touched      = false;
        for (int dir = 0; dir < 4; ++dir) {
            int x = cx, y = cy;
            while (true) {
                x += UX[dir];
                y += UY[dir];
                if (!insideN(x, y)) break;
                int nid = id(x, y);
                if (!confirmed[nid]) {
                    confirmed[nid] = 1;
                    if (dist_valid && b[x][y] == 'T' && dist[nid] != INF) touched = true;
                }
                if (b[x][y] == 'T') break;
            }
        }
        return touched;
    };

    int cx = sx, cy = sy;
    confirmed[id(cx, cy)] = 1;
    int target_id = -1;
    const int HARD_CAP = max(1, hard_cap_mul) * NN;
    int turns          = 0;

    while (turns < HARD_CAP) {
        if (cx == gx && cy == gy) return {turns, true};

        if (reveal_from(cx, cy)) dist_valid = false;

        if (confirmed[goal_id] && target_id != goal_id) {
            target_id = goal_id;
            dist_valid = false;
        }

        int cur_id = id(cx, cy);
        int stamp  = bfs_reachable(cur_id);

        if (target_id != -1) {
            if (visited[target_id] != stamp || (target_id != goal_id && confirmed[target_id])) {
                target_id = -1;
                dist_valid = false;
            }
        }

        if (target_id == -1) {
            if (confirmed[goal_id]) {
                target_id = goal_id;
            } else {
                if (unconf.empty()) return {turns, false};
                target_id = unconf[rng.randint((int)unconf.size())];
            }
            dist_valid = false;
        }

        if (!dist_valid) {
            if (!recompute_dist(target_id, cur_id)) return {turns, false};
            dist_valid = true;
        }

        int dcur = dist[cur_id];
        if (dcur >= INF) return {turns, false};

        bool moved = false;
        for (int pr = 0; pr < 4; ++pr) {
            int nx = cx + UX[pr], ny = cy + UY[pr];
            if (!insideN(nx, ny)) continue;
            if (b[nx][ny] != '.') continue;
            int nid = id(nx, ny);
            if (dist[nid] < dcur) {
                cx = nx;
                cy = ny;
                confirmed[nid] = 1;
                moved = true;
                break;
            }
        }
        if (!moved) return {turns, false};

        ++turns;
    }
    return {turns, false};
}

// ==== NEW: ターン数ベースの評価（乱択のブレは固定シードで抑える） ====
// ==== 置換: ターン数ベース評価（連結＆ループのハード制約つき） ====
static inline double eval_by_sim(const vector<vector<char>> &b, const vector<vector<char>> &orig, int sx,
                                 int sy, int gx, int gy, uint64_t base_seed = 1145141919810ull,
                                 int samples = 1) {
    // 幅1: 2x2 は即失格
    if (count_2x2(b) > 0) return -1e90;

    ConnectivityInfo conn = analyze_connectivity(b, sx, sy, gx, gy);
    if (!conn.valid) return -1e90;

    // NEW: ループ最低本数（完全な木を禁止）
    const int LOOPS_MIN  = 1;   // ここを 2,3… に上げると「よりループ多め」を強制
    const int LOOPS_GOAL = 200; // ここまで加点（それ以上は飽和）
    const double W_LOOP  = 1.5; // ループ加点の重み（控えめ）

    int loops = conn.loops;
    if (loops < LOOPS_MIN) return -1e90; // 木は不許可

    // 冒険者シミュレーション（ターン数の平均）
    long long sum_turns = 0;
    for (int s = 0; s < samples; s++) {
        uint64_t seed = base_seed + 0x9e3779b97f4a7c15ull * s;
        auto r        = simulate_adventurer(b, sx, sy, gx, gy, seed);
        if (!r.reached) return -1e90; // 念のため（all_open でほぼ弾ける）
        sum_turns += r.turns;
    }
    double mean_turns = (double)sum_turns / samples;

    // NEW: 追加T4の罰（でこぼこ維持）
    const double P_T4 = 500.0;
    int t4            = count_added_T4(b, orig);

    // ループは「ある程度」まで加点（飽和）
    double loop_bonus = W_LOOP * min(loops, LOOPS_GOAL);

    return mean_turns + loop_bonus - P_T4 * t4;
}
// 追加T4を“主成分に接続できる場合だけ”開放する
// - MAX_TUNNEL: 1〜3 くらい（直線に掘る最大長）
// - OPEN_BUDGET: 全体で最大何回成功させるか
// - LOOPS_MIN: ループ最低本数（完全な木を避ける）
void clean_added_T4_safe(vector<vector<char>> &b, const vector<vector<char>> &orig, int sx, int sy,
                         int gx, int gy, int MAX_TUNNEL = 2, int OPEN_BUDGET = 50, int LOOPS_MIN = 1) {
    const int N = (int)b.size();

    auto is_added_T4 = [&](int x, int y) -> bool {
        return b[x][y] == 'T' && orig[x][y] == '.' && is_T4(b, x, y);
    };

    auto apply_flips = [&](const vector<pair<int, int>> &cells) -> bool {
        // 反映
        vector<char> old;
        old.reserve(cells.size());
        for (auto [x, y] : cells) {
            old.push_back(b[x][y]);
            b[x][y] = '.';
        }

        bool ok = true;
        if (count_2x2(b) > 0) ok = false;
        else {
            ConnectivityInfo ci = analyze_connectivity(b, sx, sy, gx, gy);
            ok                   = ci.valid && ci.loops >= LOOPS_MIN;
        }

        if (!ok) {
            // ロールバック
            for (int k = 0; k < (int)cells.size(); k++) {
                auto [x, y] = cells[k];
                b[x][y]     = old[k];
            }
        }
        return ok;
    };

    int opened    = 0;
    bool progress = true;
    while (progress && opened < OPEN_BUDGET) {
        progress = false;

        // 候補を集める（毎ラウンド更新）
        vector<pair<int, int>> cand;
        cand.reserve(N * N);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                if (is_added_T4(i, j)) cand.emplace_back(i, j);
            }
        if (cand.empty()) break;

        // 各T4について、4方向の直線トンネル（長さ1..MAX_TUNNEL）を試す
        for (auto [x, y] : cand) {
            bool done = false;
            for (int d = 0; d < 4 && !done; ++d) {
                vector<pair<int, int>> flips;
                flips.emplace_back(x, y); // T4自身を開ける

                int cx = x, cy = y;
                for (int step = 1; step <= MAX_TUNNEL && !done; ++step) {
                    int nx = cx + dx[d], ny = cy + dy[d];
                    if (!inside(nx, ny, N, N)) break;

                    // 既に '.' に到達したら接続できたので試す
                    if (b[nx][ny] == '.') {
                        // flips には T4 自身 + 途中のトンネル（0本でも可）が入っている
                        if (apply_flips(flips)) {
                            opened++;
                            progress = true;
                            done     = true;
                        }
                        break;
                    }

                    // トンネルを延ばすには、そのマスが可変かつ現在 'T'
                    if (orig[nx][ny] != '.' || b[nx][ny] != 'T') break;
                    flips.emplace_back(nx, ny);
                    cx = nx;
                    cy = ny;

                    // 次で '.' に当たる可能性もあるのでループ継続
                    if (step == MAX_TUNNEL) {
                        // 最長地点で隣が '.' なら試す（次の while 外で評価される）
                        // ただしここで直に接続していなければ無視
                        int tx = cx + dx[d], ty = cy + dy[d];
                        if (inside(tx, ty, N, N) && b[tx][ty] == '.') {
                            if (apply_flips(flips)) {
                                opened++;
                                progress = true;
                                done     = true;
                            }
                        }
                    }
                }
            }
            if (opened >= OPEN_BUDGET) break;
        }
    }
}

// 焼きなまし本体
// ==== NEW: 焼きなまし（評価＝冒険者ターン数） ====
void make_maze(vector<vector<char>> &b, int sx, int sy, int gx, int gy,
               const vector<vector<char>> &orig) {
    struct SAParams {
        double TIME_LIMIT = 1.2, T0 = 0.8, T1 = 0.02;
    } P;
    int N = (int)b.size();

    // 変更可能セル（orig=='.' かつ S/G 以外）
    vector<pair<int, int>> mut;
    mut.reserve(N * N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (orig[i][j] == '.' && !(i == sx && j == sy) && !(i == gx && j == gy))
                mut.push_back({i, j});
    if (mut.empty()) return;

    // 2x2 をまず潰す（既存の関数を使うならそれでOK）
    // ※無ければこの行をコメントアウト
    greedy_fix_2x2(b, orig, sx, sy, gx, gy);
    repair_connectivity_local(b, orig, sx, sy, gx, gy, /*LOOPS_MIN=*/1);

    auto now = chrono::steady_clock::now;
    auto t0  = now();

    // 評価：まずはサンプル1で高速に
    const int EVAL_SAMPLES = 1;
    const int BEST_SAMPLES = 3; // ベスト更新時だけ安定化

    double cur = eval_by_sim(b, orig, sx, sy, gx, gy, /*seed*/ 114514, /*samples*/ EVAL_SAMPLES);
    vector<vector<char>> best_b = b;
    double best                 = cur;

    while (true) {
        double t = chrono::duration<double>(now() - t0).count();
        if (t > P.TIME_LIMIT) break;
        double ratio = t / P.TIME_LIMIT;
        double Temp  = P.T0 * pow(P.T1 / P.T0, ratio);

        // 変異選択：30%で「追加T4を開放したいセル」を狙う
        pair<int, int> pick;
        if ((rng64() & ((1ull << 53) - 1)) * (1.0 / 9007199254740992.0) < 0.30) {
            bool found = false;
            for (int tries = 0; tries < 16; ++tries) {
                auto p = mut[(size_t)(rng64() % mut.size())];
                if (b[p.first][p.second] == 'T' && orig[p.first][p.second] == '.' &&
                    is_T4(b, p.first, p.second)) {
                    pick  = p;
                    found = true;
                    break;
                }
            }
            if (!found) pick = mut[(size_t)(rng64() % mut.size())];
        } else {
            pick = mut[(size_t)(rng64() % mut.size())];
        }

        auto [x, y] = pick;
        char old    = b[x][y];
        b[x][y]     = (b[x][y] == '.' ? 'T' : '.');

        double nxt  = eval_by_sim(b, orig, sx, sy, gx, gy, /*seed*/ 1919810, /*samples*/ EVAL_SAMPLES);
        double diff = nxt - cur;
        bool accept = (diff >= 0) || (exp(diff / Temp) > ((rng64() >> 11) * (1.0 / 9007199254740992.0)));

        if (accept) {
            cur = nxt;
            if (nxt > best) {
                // ベスト候補はサンプル数を増やして再評価（ノイズ低減）
                double stable =
                    eval_by_sim(b, orig, sx, sy, gx, gy, /*seed*/ 7777777, /*samples*/ BEST_SAMPLES);
                if (stable > best) {
                    best   = stable;
                    best_b = b;
                }
            }
        } else {
            b[x][y] = old; // ロールバック
        }
    }

    // ベスト解に反映＆T4掃除（開けるだけなので安全）
    b = best_b;
    clean_added_T4_safe(b, orig, sx, sy, gx, gy,
                        /*MAX_TUNNEL=*/6,
                        /*OPEN_BUDGET=*/min(50, (int)b.size() * 2),
                        /*LOOPS_MIN=*/1);
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
