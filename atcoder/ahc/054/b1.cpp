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

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

// S→G が到達可能か（S/G は常に通路扱い）
bool path_exists_to_G(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int h = b.size(), w = b[0].size();
    auto inside = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };
    if (!inside(sx, sy) || !inside(gx, gy)) return false;
    if (b[sx][sy] == 'T' || b[gx][gy] == 'T') return false; // S/G は '.' 前提

    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    vis[sx][sy] = 1;
    q.push({sx, sy});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) return true;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (!inside(nx, ny) || vis[nx][ny]) continue;
            if (b[nx][ny] == 'T') continue;
            vis[nx][ny] = 1;
            q.push({nx, ny});
        }
    }
    return false;
}

// S から見て、'.'（= 'T' 以外）が全部1成分かを判定
bool all_open_connected(const vector<vector<char>> &b, int sx, int sy) {
    int h = (int)b.size(), w = (int)b[0].size();
    auto inside = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };

    if (!inside(sx, sy) || b[sx][sy] == 'T') return false;

    int total = 0;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            if (b[x][y] != 'T') ++total;

    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    vis[sx][sy] = 1;
    q.push({sx, sy});
    int reached = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        ++reached;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && !vis[nx][ny] && b[nx][ny] != 'T') {
                vis[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
    }
    return reached == total;
}

// S→G の最短距離（到達不可なら大きい値）
int shortest_path_len(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int h = b.size(), w = b[0].size();
    auto inside   = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };
    const int INF = 1e9;
    if (!inside(sx, sy) || !inside(gx, gy) || b[sx][sy] == 'T' || b[gx][gy] == 'T') return INF;

    vector<vector<int>> dist(h, vector<int>(w, INF));
    queue<pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) return dist[x][y];
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && b[nx][ny] != 'T' && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    return INF;
}

// 1回の「壁伸ばし」トライ：seed (sx,sy) からランダム方向に“既存壁に当たるまで”壁化候補を集め、
// S→G が切れないなら確定、切れるならロールバック。
bool try_wall_extend(vector<vector<char>> &b, int from_x, int from_y, int sx, int sy, int gx, int gy,
                     mt19937_64 &rng, int &curr_len) {
    int h = b.size(), w = b[0].size();
    if (!inside(from_x, from_y, w, h)) return false;
    if ((from_x == sx && from_y == sy) || (from_x == gx && from_y == gy)) return false; // ← 修正
    if (b[from_x][from_y] == 'T') return false;

    array<int, 4> ord{0, 1, 2, 3};
    shuffle(ord.begin(), ord.end(), rng);

    auto adjT = [&](int x, int y) {
        int c = 0;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny, w, h) && b[nx][ny] == 'T') ++c;
        }
        return c;
    };

    rep(k, 4) {
        int d = ord[k];
        vector<pair<int, int>> mods;
        int x = from_x, y = from_y;
        while (true) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (!inside(nx, ny, w, h)) break;
            if ((nx == sx && ny == sy) || (nx == gx && ny == gy)) break;
            if (b[nx][ny] == 'T') break;
            mods.emplace_back(nx, ny);
            x = nx;
            y = ny;
        }
        if (mods.empty()) continue;

        int committed = 0;
        for (auto [ux, uy] : mods) {
            // 置く前に「周囲に壁が2つ以上」なら打ち止め（無駄置き回避）
            if (adjT(ux, uy) >= 2) break;

            b[ux][uy] = 'T';                      // 仮置き
            if (!all_open_connected(b, sx, sy)) { // 全通路連結を維持
                b[ux][uy] = '.';                  // 壊れるなら戻す
                break;
            }
            int nl = shortest_path_len(b, sx, sy, gx, gy);
            if (nl < curr_len) { // 最短距離が縮むなら却下
                b[ux][uy] = '.';
                break;
            }
            ++committed; // セーフ
            curr_len = nl;
        }
        if (committed > 0) return true;
    }
    return false;
}

// 壁伸ばし本体：iters 回ランダムに試す
void wall_extend(vector<vector<char>> &b, int sx, int sy, int gx, int gy, int iters = 6000,
                 uint64_t seed = 123456789ULL) {
    int h = b.size(), w = b[0].size();
    mt19937_64 rng(seed);

    uniform_int_distribution<int> dxrand(0, w - 1), dyrand(0, h - 1);

    int curr_len = shortest_path_len(b, sx, sy, gx, gy);

    rep(it, iters) {
        int from_x = dxrand(rng);
        int from_y = dyrand(rng);
        // 通路から始める（既存壁からでも書けるが、通路起点のほうが“埋める”効果が高い）
        if (b[from_x][from_y] == 'T') continue;
        ll cnt = 0;
        // 少しだけ「壁の近傍」を優先すると伸びやすい
        bool nearWall = false;
        ll to_x, to_y;
        rep(d, 4) {
            int nx = from_x + dx[d], ny = from_y + dy[d];
            if (inside(nx, ny, w, h) && b[nx][ny] == 'T') {
                nearWall = true;
                cnt++;
            }
        }
        if (!nearWall) continue;
        if (cnt >= 2) continue;

        (void)try_wall_extend(b, from_x, from_y, sx, sy, gx, gy, rng, curr_len);
    }
}

// ===== ここからコピペ =====
static const int INF_INT2 = 1e9;

// S=(sx,sy) からの距離（'.'のみ通れる）
vector<vector<int>> bfs_dist_from(const vector<vector<char>> &b, int sx, int sy) {
    int N = (int)b.size();
    vector<vector<int>> dist(N, vector<int>(N, INF_INT2));
    auto inside2 = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    if (!inside2(sx, sy) || b[sx][sy] == 'T') return dist;
    queue<pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (!inside2(nx, ny) || b[nx][ny] == 'T') continue;
            if (dist[nx][ny] != INF_INT2) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}

// 任意の最短路上判定
inline bool on_any_shortest_path_xy(const vector<vector<int>> &distS, const vector<vector<int>> &distG,
                                    int gx, int gy, int x, int y) {
    int dS = distS[x][y], dG = distG[x][y], dSG = distS[gx][gy];
    if (dS >= INF_INT2 || dG >= INF_INT2 || dSG >= INF_INT2) return false;
    return (dS + dG == dSG);
}

// deg 行列と μ を構築（μ = E − V + 1、ここでは連結1成分前提）
struct OpenStats {
    vector<vector<int>> deg;
    int V = 0, E = 0, mu = 0;
};
OpenStats build_deg_mu(const vector<vector<char>> &b) {
    int N = (int)b.size();
    OpenStats st;
    st.deg.assign(N, vector<int>(N, 0));
    auto inside2 = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            if (b[x][y] == 'T') continue;
            ++st.V;
            int c = 0;
            for (int d = 0; d < 4; ++d) {
                int nx = x + (int)dx[d], ny = y + (int)dy[d];
                if (inside2(nx, ny) && b[nx][ny] != 'T') ++c;
            }
            st.deg[x][y] = c;
            st.E += c;
        }
    st.E /= 2;               // 無向
    st.mu = st.E - st.V + 1; // 連結成分=1を仮定
    return st;
}

// 関節点マスク（Tarjan、'.'グラフ, 4近傍）
vector<vector<char>> articulation_mask_xy(const vector<vector<char>> &b, int sx, int sy) {
    int N = (int)b.size();
    vector<vector<char>> art(N, vector<char>(N, 0));
    auto inside2 = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    if (!inside2(sx, sy) || b[sx][sy] == 'T') return art;

    int T = 0;
    vector<int> disc(N * N, -1), low(N * N, -1), par(N * N, -1);
    auto id = [&](int x, int y) { return x * N + y; };
    auto XY = [&](int v) { return pair<int, int>(v / N, v % N); };

    function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = ++T;
        auto [ux, uy]    = XY(u);
        int child        = 0;
        for (int d = 0; d < 4; ++d) {
            int vx = ux + (int)dx[d], vy = uy + (int)dy[d];
            if (!inside2(vx, vy) || b[vx][vy] == 'T') continue;
            int v = id(vx, vy);
            if (disc[v] == -1) {
                par[v] = u;
                ++child;
                dfs(v);
                low[u] = min(low[u], low[v]);
                if (par[u] == -1 && child > 1) art[ux][uy] = 1;
                if (par[u] != -1 && low[v] >= disc[u]) art[ux][uy] = 1;
            } else if (v != par[u]) {
                low[u] = min(low[u], disc[v]);
            }
        }
    };
    dfs(id(sx, sy));
    return art;
}

// 現在の最短距離（到達不可＝INF_INT2）
int shortest_len_xy(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    auto distS = bfs_dist_from(b, sx, sy);
    return distS[gx][gy];
}

// 無駄（サイクル）を削りつつ S→G の最短距離を維持：
// - deg>=2 かつ 関節点でない '.' を優先的に 'T' にする
// - S→G最短路に「乗ってない」候補は距離再計算をスキップ（基本距離不変）
// - 受理したときだけ dist/関節点を更新
void prune_cycles_preserve_distance(vector<vector<char>> &b, int sx, int sy, int gx, int gy,
                                    int passes = 2, uint64_t seed = 20250921ULL) {
    int N = (int)b.size();
    mt19937_64 rng(seed);

    // 初期情報
    auto stats   = build_deg_mu(b);
    auto distS   = bfs_dist_from(b, sx, sy);
    auto distG   = bfs_dist_from(b, gx, gy);
    int curr_len = distS[gx][gy];
    if (curr_len >= INF_INT2) return; // そもそも不達なら何もしない
    auto art = articulation_mask_xy(b, sx, sy);

    // 候補列
    vector<pair<int, int>> cand;
    cand.reserve(N * N);
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            if (b[x][y] != 'T' && !(x == sx && y == sy) && !(x == gx && y == gy))
                cand.emplace_back(x, y);
        }

    for (int p = 0; p < passes; ++p) {
        shuffle(all(cand), rng);
        bool progressed = false;

        for (auto [x, y] : cand) {
            if (b[x][y] == 'T') continue;
            if (art[x][y]) continue; // 関節点は触らない（連結維持）
            int dg = stats.deg[x][y];
            if (dg <= 1) continue; // 葉は削ってもサイクル数は減らない（μ変化0）

            bool on_sp = on_any_shortest_path_xy(distS, distG, gx, gy, x, y);

            // 一時的に塞ぐ
            b[x][y] = 'T';
            // 近傍の deg と μ を差分更新
            int delta_mu = -(dg - 1); // ノード削除で μ ← μ - (deg-1)
            stats.mu += delta_mu;
            stats.V -= 1;
            stats.E -= dg;
            // 周辺4マスの deg を1だけ下げる
            for (int d = 0; d < 4; ++d) {
                int nx = x + (int)dx[d], ny = y + (int)dy[d];
                if (0 <= nx && nx < N && 0 <= ny && ny < N && b[nx][ny] != 'T') {
                    stats.deg[nx][ny] -= 1;
                }
            }

            // S→G 距離チェック（最短路に乗っているときだけ再計算）
            bool ok = true;
            if (on_sp) {
                auto tmp = bfs_dist_from(b, sx, sy);
                int nl   = tmp[gx][gy];
                if (nl < curr_len) {
                    ok = false;
                } // 距離が縮むのはダメ
                else {
                    // 受理：情報を更新
                    distS    = std::move(tmp);
                    distG    = bfs_dist_from(b, gx, gy);
                    curr_len = nl;
                }
            }
            if (ok) {
                progressed = true;
                // 関節点マスクは採用時のみ更新（コスト重いので）
                art = articulation_mask_xy(b, sx, sy);
            } else {
                // 却下：ロールバック
                b[x][y] = '.';
                // μ,E,V,deg 戻す
                stats.mu -= delta_mu;
                stats.V += 1;
                stats.E += dg;
                for (int d = 0; d < 4; ++d) {
                    int nx = x + (int)dx[d], ny = y + (int)dy[d];
                    if (0 <= nx && nx < N && 0 <= ny && ny < N && b[nx][ny] != 'T') {
                        stats.deg[nx][ny] += 1;
                    }
                }
            }
        }

        // もうサイクルが無ければ終了（完全に“無駄なし”＝木構造）
        if (stats.mu <= 0) break;
        if (!progressed) break;
    }
}
// ===== ここまでコピペ =====
// ========= ここからコピペ（C++17） =========

// ノードID <-> (x,y)
static inline int vid(int x, int y, int N) { return x * N + y; }
static inline pair<int, int> XY(int v, int N) { return {v / N, v % N}; }

// 木の BFS で S→G 距離と親を計算
static int tree_dist_with_parent(const vector<vector<int>> &tadj, int s, int g, vector<int> &par) {
    int V = (int)tadj.size();
    par.assign(V, -1);
    queue<int> q;
    q.push(s);
    par[s] = -2;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == g) {
            // 距離を数える
            int len = 0, cur = g;
            while (par[cur] != -2) {
                cur = par[cur];
                ++len;
            }
            return len;
        }
        for (int v : tadj[u]) {
            if (par[v] != -1) continue;
            par[v] = u;
            q.push(v);
        }
    }
    return -1;
}

// 木の S→G 経路のノード集合（id のフラグ）
static vector<char> build_path_mask(const vector<vector<int>> &tadj, int s, int g) {
    vector<int> par;
    int d = tree_dist_with_parent(tadj, s, g, par);
    vector<char> on(tadj.size(), 0);
    if (d < 0) return on;
    int cur = g;
    on[cur] = 1;
    while (par[cur] != -2) {
        cur     = par[cur];
        on[cur] = 1;
    }
    return on;
}

// 木の辺を一時的に切る/繋ぐヘルパ
static inline void add_edge(vector<vector<int>> &adj, int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
}
static inline void remove_edge(vector<vector<int>> &adj, int a, int b) {
    auto &A = adj[a];
    auto &B = adj[b];
    A.erase(remove(A.begin(), A.end(), b), A.end());
    B.erase(remove(B.begin(), B.end(), a), B.end());
}

// 辺キー（無向）: min<<32 | max
static inline uint64_t ekey(int a, int b) {
    if (a > b) swap(a, b);
    return ((uint64_t)a << 32) | (uint32_t)b;
}

// 盤面上の '.' 連結成分（Sと同じ成分）を集める
static vector<int> collect_component(const vector<vector<char>> &b, int sx, int sy) {
    int N = (int)b.size();
    vector<int> comp;
    vector<char> vis(N * N, 0);
    auto inside = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    queue<pair<int, int>> q;
    if (!inside(sx, sy) || b[sx][sy] == 'T') return comp;
    q.push({sx, sy});
    vis[vid(sx, sy, N)] = 1;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        comp.push_back(vid(x, y, N));
        for (int d = 0; d < 4; ++d) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (!inside(nx, ny) || b[nx][ny] == 'T') continue;
            int id = vid(nx, ny, N);
            if (vis[id]) continue;
            vis[id] = 1;
            q.push({nx, ny});
        }
    }
    return comp;
}

// 全開通辺（'.' 同士の隣接）を列挙（u<v に正規化）
static vector<pair<int, int>> all_open_edges(const vector<vector<char>> &b, const vector<int> &nodes) {
    int N = (int)b.size();
    vector<char> is(N * N, 0);
    for (int v : nodes) is[v] = 1;
    vector<pair<int, int>> E;
    for (int v : nodes) {
        auto [x, y] = XY(v, N);
        // 右・下だけ見れば重複しない
        if (x + 1 < N && b[x + 1][y] != 'T' && is[vid(x + 1, y, N)]) E.push_back({v, vid(x + 1, y, N)});
        if (y + 1 < N && b[x][y + 1] != 'T' && is[vid(x, y + 1, N)]) E.push_back({v, vid(x, y + 1, N)});
    }
    return E;
}

// 初期の全域木：ランダム DFS（S 起点）
static vector<vector<int>> build_random_tree(const vector<vector<char>> &b, const vector<int> &comp,
                                             int s_id, uint64_t seed) {
    int N = (int)b.size(), Vcnt = (int)comp.size();
    vector<char> in(N * N, 0);
    for (int v : comp) in[v] = 1;
    vector<vector<int>> adjTree(N * N); // スパースだが N*N で持つ
    vector<char> used(N * N, 0);
    mt19937_64 rng(seed);

    vector<int> order = comp;
    shuffle(order.begin(), order.end(), rng); // 近傍順のランダム化用

    // 近傍順をシャッフルしつつ DFS
    stack<int> st;
    st.push(s_id);
    used[s_id]  = 1;
    auto inside = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    while (!st.empty()) {
        int u         = st.top();
        auto [ux, uy] = XY(u, N);
        // ランダム順
        array<int, 4> dir{0, 1, 2, 3};
        shuffle(dir.begin(), dir.end(), rng);
        bool pushed = false;
        for (int k = 0; k < 4; ++k) {
            int d  = dir[k];
            int vx = ux + (int)dx[d], vy = uy + (int)dy[d];
            if (!inside(vx, vy)) continue;
            int v = vid(vx, vy, N);
            if (!in[v] || used[v]) continue;
            // 木に追加
            add_edge(adjTree, u, v);
            used[v] = 1;
            st.push(v);
            pushed = true;
            break;
        }
        if (!pushed) st.pop();
    }
    return adjTree;
}

// 木のサイズ（含まれるノード数）
static int tree_size(const vector<vector<int>> &tadj) {
    int cnt = 0;
    for (auto &v : tadj)
        if (!v.empty()) ++cnt;
    return cnt; // 根拠弱いが、同じ V を共有している前提で OK（少なくとも S 成分は埋まってる）
}

// u-v を追加したときにできるサイクル上のノード列（木上の u→v 経路）
static vector<int> tree_path_uv(const vector<vector<int>> &tadj, int u, int v) {
    int V = (int)tadj.size();
    vector<int> par(V, -1);
    queue<int> q;
    q.push(u);
    par[u] = -2;
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        if (x == v) break;
        for (int y : tadj[x]) {
            if (par[y] != -1) continue;
            par[y] = x;
            q.push(y);
        }
    }
    vector<int> path;
    if (par[v] == -1) return path;
    int cur = v;
    while (cur != -2) {
        path.push_back(cur);
        cur = par[cur];
    }
    reverse(path.begin(), path.end());
    return path; // u ... v
}

// SA（というより貪欲＋確率受容）で S→G 距離を伸ばす
void optimize_tree_SG_distance(vector<vector<int>> &tadj, const vector<pair<int, int>> &allE, // 全 '.'
                               int s_id, int g_id, int iters = 4000, double T0 = 0.15,
                               uint64_t seed = 20250921ULL) {
    mt19937_64 rng(seed);
    int V = (int)tadj.size();

    // 木辺集合・非木辺集合
    unordered_set<uint64_t> treeE, nonE;
    treeE.reserve(allE.size() * 2);
    nonE.reserve(allE.size() * 2);
    auto isTreeEdge = [&](int a, int b) {
        if (a > b) swap(a, b);
        // O(deg)で探してもOKだが set 化しておく
        return treeE.count(ekey(a, b));
    };

    // 初期登録
    for (int u = 0; u < V; ++u)
        for (int v : tadj[u])
            if (u < v) treeE.insert(ekey(u, v));
    for (auto [a, b] : allE) {
        uint64_t k = ekey(a, b);
        if (!treeE.count(k)) nonE.insert(k);
    }

    // 現在の距離
    vector<int> par;
    int bestL = tree_dist_with_parent(tadj, s_id, g_id, par);
    if (bestL < 0) return;

    uniform_real_distribution<double> U(0.0, 1.0);

    for (int it = 0; it < iters; ++it) {
        if (nonE.empty()) break;
        // 温度（緩やかに冷やす）
        double T = T0 * pow(0.995, it);

        // ランダムな非木辺（u,v）をピック
        auto itNon = nonE.begin();
        advance(itNon, rng() % nonE.size());
        uint64_t ek = *itNon;
        int u = (int)(ek >> 32), v = (int)(ek & 0xffffffffu);

        // 木上の u→v 経路（サイクル）
        auto cyc = tree_path_uv(tadj, u, v);
        if (cyc.size() < 2) continue;

        // 候補として、経路の端/中央付近の数個の辺を試す
        vector<int> idxs;
        idxs.push_back(0);
        if ((int)cyc.size() >= 3) idxs.push_back(1);
        idxs.push_back((int)cyc.size() - 2);
        idxs.push_back((int)cyc.size() - 1);
        idxs.push_back((int)cyc.size() / 2);
        sort(idxs.begin(), idxs.end());
        idxs.erase(unique(idxs.begin(), idxs.end()), idxs.end());

        // 最良を探す（改善が見つかったら即採用でもOK）
        int pick_a = -1, pick_b = -1, newL = bestL, rem_idx = -1;
        for (int t = 0; t < (int)idxs.size() - 1; ++t) {
            int a = cyc[idxs[t]], b = cyc[idxs[t] + 1];
            if (isTreeEdge(a, b) == false) continue; // 念のため
            // 仮 flip: add (u,v), remove (a,b)
            add_edge(tadj, u, v);
            remove_edge(tadj, a, b);
            vector<int> par2;
            int L2 = tree_dist_with_parent(tadj, s_id, g_id, par2);
            // ロールバック
            add_edge(tadj, a, b);
            remove_edge(tadj, u, v);

            if (L2 > newL) {
                newL    = L2;
                pick_a  = a;
                pick_b  = b;
                rem_idx = idxs[t];
            } else {
                // SA的受容
                double delta = (double)(L2 - bestL);
                if (delta < 0 && U(rng) < exp(delta / max(1e-9, T))) {
                    newL    = L2;
                    pick_a  = a;
                    pick_b  = b;
                    rem_idx = idxs[t];
                }
            }
        }

        if (pick_a == -1) continue; // 受容なし

        // 本当に flip する
        add_edge(tadj, u, v);
        remove_edge(tadj, pick_a, pick_b);

        // 集合更新
        uint64_t k_add = ekey(u, v);
        uint64_t k_del = ekey(pick_a, pick_b);
        treeE.erase(k_del);
        treeE.insert(k_add);
        nonE.erase(k_add);
        nonE.insert(k_del);

        bestL = newL;
    }
}

// ========== ここからコピペ ==========
static inline int dir_between(int ax, int ay, int bx, int by) {
    if (bx == ax && by == ay + 1) return 0; // down (dy=+1)
    if (bx == ax + 1 && by == ay) return 1; // right
    if (bx == ax && by == ay - 1) return 2; // up
    if (bx == ax - 1 && by == ay) return 3; // left
    return 0;
}

// 経路ベクタ（s→g の順）を作る
static vector<int> restore_path_vec(const vector<vector<int>> &tadj, int s, int g) {
    vector<int> par;
    int d = tree_dist_with_parent(tadj, s, g, par);
    vector<int> path;
    if (d < 0) return path;
    int cur = g;
    while (cur != -2) {
        path.push_back(cur);
        cur = par[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

// 経路の各点から、直線の“葉”を生やす（サイクル禁止）
// keep: 開けておくマスク（1=開ける）; orig は元盤面（'.' だけ許可）
static void sprout_branches_straight(const vector<vector<char>> &orig, vector<vector<char>> &keep,
                                     const vector<int> &path, double p_branch, int max_branch_len,
                                     uint64_t seed) {
    int N       = (int)orig.size();
    auto inside = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    mt19937_64 rng(seed);
    uniform_real_distribution<double> U(0.0, 1.0);

    auto ok_add = [&](int x, int y, int px, int py) -> bool {
        if (!inside(x, y) || orig[x][y] == 'T') return false;
        if (keep[x][y]) return false;
        int cnt = 0;
        for (int d = 0; d < 4; ++d) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (!inside(nx, ny)) continue;
            if (keep[nx][ny]) ++cnt;
        }
        if (cnt != 1) return false; // 木を保つ（接続先は1つだけ）
        if (px != -1) {             // 直前以外の開セルと接触しない
            int c2 = 0;
            for (int d = 0; d < 4; ++d) {
                int nx = x + (int)dx[d], ny = y + (int)dy[d];
                if (!inside(nx, ny)) continue;
                if (keep[nx][ny] && !(nx == px && ny == py)) ++c2;
            }
            if (c2 > 0) return false;
        }
        return true;
    };

    vector<pair<int, int>> pxy;
    pxy.reserve(path.size());
    for (int v : path) pxy.push_back(XY(v, N));

    for (int i = 0; i < (int)pxy.size(); ++i) {
        int x = pxy[i].first, y = pxy[i].second;

        int dirF = 0;
        if (i > 0) {
            auto [px, py] = pxy[i - 1];
            dirF          = dir_between(px, py, x, y);
        } else if (i + 1 < (int)pxy.size()) {
            auto [nx, ny] = pxy[i + 1];
            dirF          = dir_between(x, y, nx, ny);
        }

        int perps[2] = {(dirF + 1) & 3, (dirF + 3) & 3};
        for (int t = 0; t < 2; ++t) {
            if (U(rng) > p_branch) continue;
            int d  = perps[t];
            int bx = x, by = y;
            int px = -1, py = -1;
            for (int step = 0; step < max_branch_len; ++step) {
                int nx = bx + (int)dx[d], ny = by + (int)dy[d];
                if (!inside(nx, ny) || orig[nx][ny] == 'T') break;
                if (!ok_add(nx, ny, bx, by)) break;
                keep[nx][ny] = 1;
                px           = bx;
                py           = by;
                bx           = nx;
                by           = ny;
                if (U(rng) < 0.25) break; // 長さに揺らぎ
            }
        }
    }
}

// SA で最長化 → 経路＋枝を残して他は閉じる
void carve_long_path_tree_SA_with_branches(vector<vector<char>> &b, const vector<vector<char>> &orig,
                                           int sx, int sy, int gx, int gy, int iters = 6000,
                                           double T0 = 0.20, uint64_t seed = 114514ULL,
                                           double p_branch = 0.65, int max_branch_len = 4) {
    int N = (int)b.size();
    if (!path_exists_to_G(b, sx, sy, gx, gy)) return;

    auto comp = collect_component(b, sx, sy);
    if (comp.empty()) return;
    int s_id = vid(sx, sy, N), g_id = vid(gx, gy, N);
    {
        vector<char> mark(N * N, 0);
        for (int v : comp) mark[v] = 1;
        if (!mark[g_id]) return;
    }
    auto Eall = all_open_edges(b, comp);
    auto tadj = build_random_tree(b, comp, s_id, seed);
    optimize_tree_SG_distance(tadj, Eall, s_id, g_id, iters, T0, seed ^ 0x9e37);

    auto path = restore_path_vec(tadj, s_id, g_id);
    if (path.empty()) return;

    vector<vector<char>> keep(N, vector<char>(N, 0));
    for (int v : path) {
        auto [x, y] = XY(v, N);
        keep[x][y]  = 1;
    }
    keep[sx][sy] = 1;
    keep[gx][gy] = 1; // 念のため
    sprout_branches_straight(/*orig=*/orig, keep, path, p_branch, max_branch_len, seed ^ 0xf00d);

    // ★ここが修正点：orig が '.' の所だけ keep に従って書く（b の現在値は無視）
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            if (orig[x][y] == 'T') {
                b[x][y] = 'T';
                continue;
            }
            b[x][y] = keep[x][y] ? '.' : 'T';
        }
}

// ========== ここまでコピペ ==========
// 木（keep=1）ができたあとに、orig=='.' で閉じているセルのうち
// 「開いたときに開近傍がちょうど1」のものを貪欲に取り込む（= 木を最大化）。
void expand_tree_to_maximal(const vector<vector<char>> &orig, vector<vector<char>> &keep) {
    const int N  = (int)orig.size();
    auto inside  = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    auto degOpen = [&](int x, int y) {
        int c = 0;
        for (int d = 0; d < 4; ++d) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (inside(nx, ny) && keep[nx][ny]) ++c;
        }
        return c;
    };

    queue<pair<int, int>> q;
    vector<vector<char>> inq(N, vector<char>(N, 0));

    // 初期候補
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            if (orig[x][y] == 'T' || keep[x][y]) continue;
            if (degOpen(x, y) == 1) {
                q.push({x, y});
                inq[x][y] = 1;
            }
        }

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        inq[x][y] = 0;
        if (orig[x][y] == 'T' || keep[x][y]) continue;
        if (degOpen(x, y) != 1) continue; // 状態が変わっていたらスキップ
        keep[x][y] = 1;                   // 葉として採用
        // 近傍も再評価
        for (int d = 0; d < 4; ++d) {
            int nx = x + (int)dx[d], ny = y + (int)dy[d];
            if (!inside(nx, ny)) continue;
            if (orig[nx][ny] == 'T' || keep[nx][ny]) continue;
            if (degOpen(nx, ny) == 1 && !inq[nx][ny]) {
                q.push({nx, ny});
                inq[nx][ny] = 1;
            }
        }
    }
}

// 距離維持でサイクルを削り、その後、木を最大化して無駄な壁をなくす。
// orig: 元の盤面（最初に読んだ b をそのまま渡す）。orig=='T' は二度と開けない。
void prune(vector<vector<char>> &b, int sx, int sy, int gx, int gy, const vector<vector<char>> &orig,
           int passes = 2, uint64_t seed = 20250921ULL) {
    // 1) 既存の距離維持プルーニング
    prune_cycles_preserve_distance(b, sx, sy, gx, gy, passes, seed);

    // 2) 現在の開セルを keep として取り出し → 木を最大化
    int N = (int)b.size();
    vector<vector<char>> keep(N, vector<char>(N, 0));
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y)
            if (b[x][y] != 'T') keep[x][y] = 1;

    expand_tree_to_maximal(/*orig=*/orig, /*keep=*/keep);

    // 念のため S/G を確実に開けておく
    keep[sx][sy] = 1;
    keep[gx][gy] = 1;

    // 3) 書き戻し：orig=='.' の範囲で keep==1 を '.'、それ以外を 'T'
    for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y) {
            if (orig[x][y] == 'T') continue; // 元壁は触らない
            b[x][y] = keep[x][y] ? '.' : 'T';
        }
}

// G を「曲がり角の奥」に隠し、入口は一本みち（P は G と外側 O だけ開）にする。
// - 既存の '.' だけを 'T' に変える（開けない）
// - S→G 到達可能性は常に維持。keep_all_connected を true にすると全通路連結も維持。
// - outward 必須なら require_outward_open=true に（O が '.' の候補を優先/必須化）
//
// 依存: dx,dy / path_exists_to_G / all_open_connected
// 入口で必ず曲がり、かつ曲がれるのは片側のみ（T字禁止）
// Gは3面壁、入口直進は封じる、角セルでも左右を塞いでT字禁止。
// 8通り（d=4方向 × 左右2方向）を総当たりし、S→G到達性が保てるパターンを採用。
// どうしても無理なら入口一本化だけ行うフォールバック。
// 返り値: 成功したら true。placed_out に「この関数で立てた壁」を入れる
bool narrow_goal_biased_toward_wall_away_from_start(
    vector<vector<char>> &b, int sx, int sy, // Start
    int gx, int gy,                          // Goal
    vector<pair<int, int>> *placed_out,      // ★追加: 立てた壁のリスト（アウト）
    double w_edge = 1.0, double w_away = 2.5) {
    const int N  = (int)b.size();
    auto inside2 = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };

    vector<pair<int, int>> touched; // ← この試行で触った壁
    auto placeWall = [&](int x, int y) {
        if (!inside2(x, y)) return;
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
    auto commit = [&]() {
        if (placed_out) {
            // 確定した壁を呼び出し元に渡す
            placed_out->insert(placed_out->end(), touched.begin(), touched.end());
        }
        touched.clear(); // この試行は確定
    };

    // —— 8通りを総当たり ——（既存実装そのまま）
    for (int d = 0; d < 4; ++d) {
        int ex = gx + (int)dx[d], ey = gy + (int)dy[d];
        if (!inside2(ex, ey) || b[ex][ey] != '.') continue;

        int leftDir   = (d + 3) & 3;
        int rightDir  = (d + 1) & 3;
        auto away_key = [&](int sdir) {
            int vx = sx - ex, vy = sy - ey; // 入口→S
            return vx * (int)dx[sdir] + vy * (int)dy[sdir];
        };
        array<int, 2> sides = {leftDir, rightDir};
        if (away_key(sides[0]) > away_key(sides[1])) swap(sides[0], sides[1]);

        for (int k = 0; k < 2; ++k) {
            int sdir = sides[k];
            int odir = sides[1 - k];
            int ox = ex + (int)dx[sdir], oy = ey + (int)dy[sdir];
            if (!inside2(ox, oy) || b[ox][oy] != '.') continue;

            touched.clear();

            // (1) Gの3面を壁化（入口側 d 以外）
            for (int dd = 0; dd < 4; ++dd) {
                if (dd == d) continue;
                placeWall(gx + (int)dx[dd], gy + (int)dy[dd]);
            }
            // (2) 入口直進を封じる
            placeWall(ex + (int)dx[d], ey + (int)dy[d]);
            // (3) 入口もう一方の横も封じて T字禁止
            placeWall(ex + (int)dx[odir], ey + (int)dy[odir]);
            // (4) 角セルでも左右を封じて T字禁止
            int l2 = (sdir + 3) & 3, r2 = (sdir + 1) & 3;
            placeWall(ox + (int)dx[l2], oy + (int)dy[l2]);
            placeWall(ox + (int)dx[r2], oy + (int)dy[r2]);

            if (path_exists_to_G(b, sx, sy, gx, gy)) {
                commit(); // ★確定（placed_out に入れる）
                return true;
            }
            rollback();
        }
    }

    // —— フォールバック：入口一本化のみ ——（既存のロジックを流用）
    struct Cand {
        int x, y, dir, score;
    };
    auto dist_edge = [&](int x, int y) { return min(min(x, N - 1 - x), min(y, N - 1 - y)); };
    vector<Cand> cands;
    for (int d = 0; d < 4; ++d) {
        int nx = gx + (int)dx[d], ny = gy + (int)dy[d];
        if (!inside2(nx, ny) || b[nx][ny] != '.') continue;
        int vx = sx - gx, vy = sy - gy;
        int dot       = (nx - gx) * vx + (ny - gy) * vy;
        int edgeScore = (N - dist_edge(nx, ny));
        cands.push_back({nx, ny, d, -dot * 1000 + edgeScore});
    }
    if (cands.size() <= 1) return false;

    sort(cands.begin(), cands.end(), [](auto &a, auto &b) { return a.score > b.score; });
    auto keep = cands.front();
    for (size_t i = 1; i < cands.size(); ++i) {
        int nx = cands[i].x, ny = cands[i].y;
        if (b[nx][ny] != '.') continue;
        char old  = b[nx][ny];
        b[nx][ny] = 'T';
        if (path_exists_to_G(b, sx, sy, gx, gy)) {
            touched.emplace_back(nx, ny);
        } else {
            b[nx][ny] = old;
        }
    }
    if (!touched.empty()) {
        commit(); // ★確定
        return true;
    }
    return false;
}

// ---- 追記ここまで ----

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int N;
    int gx, gy;
    cin >> N >> gx >> gy;
    int sx = 0, sy = N / 2; // 入口

    vector<vector<char>> b(N, vector<char>(N));
    rep(i, N) rep(j, N) cin >> b[i][j];
    vector<vector<char>> origin = b;

    vector<pair<int, int>> pinned;
    bool ok = narrow_goal_biased_toward_wall_away_from_start(b, sx, sy, gx, gy, &pinned, /*w_edge=*/1.0,
                                                             /*w_away=*/2.5);
    auto origin_hard = origin; // コピー
    for (auto [x, y] : pinned) origin_hard[x][y] = 'T';

    // “原盤面を基準に上書きする” carve/prune へは origin_hard を渡す
    carve_long_path_tree_SA_with_branches(b, origin_hard, sx, sy, gx, gy, 6000, 0.20, 114514ULL, 0.65,
                                          4);
    prune(b, /*S*/ sx, sy, /*G*/ gx, gy, /*orig=*/origin_hard,
          /*passes=*/2, /*seed=*/20250921ULL);
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
            for (auto [x, y] : ans) cout << ' ' << x << ' ' << y;
            cout << endl;
        } else {
            cout << 0 << endl;
        }
    }
}
