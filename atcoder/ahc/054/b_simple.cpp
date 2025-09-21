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

static inline int count_2x2(const vector<vector<char>> &b) {
    int N = (int)b.size();
    int c = 0;
    for (int i = 0; i + 1 < N; i++) {
        for (int j = 0; j + 1 < N; j++) {
            if (b[i][j] == '.' && b[i + 1][j] == '.' && b[i][j + 1] == '.' && b[i + 1][j + 1] == '.')
                c++;
        }
    }
    return c;
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