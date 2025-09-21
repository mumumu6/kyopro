// g++ -O2 -std=gnu++17 -Wall -Wextra -o solve solve.cpp
#include <bits/stdc++.h>
using namespace std;

using ll                 = long long;
static const int INF_INT = 1e9;

#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define all(v) (v).begin(), (v).end()
#define el '\n'

// 4近傍（下,右,上,左）※順番に意味はない
static const int dy[4] = {+1, 0, -1, 0};
static const int dx[4] = {0, +1, 0, -1};

// ========== ユーティリティ ==========
static inline bool inside(int x, int y, int W, int H) { return 0 <= x && x < W && 0 <= y && y < H; }

// S→G が到達可能か（S/G は '.' 前提・b[y][x]）
bool path_exists_to_G(const vector<vector<char>> &b, int si, int sj, int gi, int gj) {
    const int H = (int)b.size(), W = (int)b[0].size();
    if (!inside(sj, si, W, H) || !inside(gj, gi, W, H)) return false;
    if (b[si][sj] == 'T' || b[gi][gj] == 'T') return false;

    vector<vector<char>> vis(H, vector<char>(W, 0));
    queue<pair<int, int>> q;
    vis[si][sj] = 1;
    q.push({si, sj});
    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        if (y == gi && x == gj) return true;
        for (int d = 0; d < 4; ++d) {
            int ny = y + dy[d], nx = x + dx[d];
            if (!inside(nx, ny, W, H)) continue;
            if (vis[ny][nx] || b[ny][nx] == 'T') continue;
            vis[ny][nx] = 1;
            q.push({ny, nx});
        }
    }
    return false;
}

// G の入口を1本に（削っても S→G 可なら確定）
void narrow_goal(vector<vector<char>> &b, int si, int sj, int gi, int gj) {
    const int H = (int)b.size(), W = (int)b[0].size();
    vector<pair<int, int>> openN;
    for (int d = 0; d < 4; ++d) {
        int ny = gi + dy[d], nx = gj + dx[d];
        if (inside(nx, ny, W, H) && b[ny][nx] != 'T') openN.emplace_back(ny, nx);
    }
    if ((int)openN.size() <= 1) return;

    mt19937_64 rng(20240921ULL);
    shuffle(openN.begin(), openN.end(), rng);
    for (auto [ny, nx] : openN) {
        if ((int)openN.size() <= 1) break;
        char keep = b[ny][nx];
        b[ny][nx] = 'T';
        if (!path_exists_to_G(b, si, sj, gi, gj)) {
            b[ny][nx] = keep;
        } else { // 確定で一本減った
            // 何もせず続行（次の候補も削ってよければ削る）
        }
    }
}

// ========== 距離BFS ==========
vector<vector<int>> compute_dist_from(const vector<vector<char>> &b, int si, int sj) {
    const int H = (int)b.size(), W = (int)b[0].size();
    vector<vector<int>> dist(H, vector<int>(W, INF_INT));
    if (!inside(sj, si, W, H) || b[si][sj] == 'T') return dist;
    queue<pair<int, int>> q;
    dist[si][sj] = 0;
    q.push({si, sj});
    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int ny = y + dy[d], nx = x + dx[d];
            if (!inside(nx, ny, W, H)) continue;
            if (b[ny][nx] == 'T') continue;
            if (dist[ny][nx] != INF_INT) continue;
            dist[ny][nx] = dist[y][x] + 1;
            q.push({ny, nx});
        }
    }
    return dist;
}

// 最短路上に乗っているか（任意の最短路）
inline bool on_any_shortest_path(const vector<vector<int>> &distS, const vector<vector<int>> &distG,
                                 int gi, int gj, int y, int x) {
    int dS = distS[y][x], dG = distG[y][x];
    int dSG = distS[gi][gj];
    if (dS >= INF_INT || dG >= INF_INT || dSG >= INF_INT) return false;
    return (dS + dG == dSG);
}

// 最短距離は固定とみなし、最短路DAG上でターン最小を0-1 BFS
int min_turns_on_shortest_paths(const vector<vector<char>> &b, const vector<vector<int>> &dist, int si,
                                int sj, int gi, int gj) {
    const int H = (int)b.size(), W = (int)b[0].size();
    if (dist[gi][gj] >= INF_INT) return INF_INT;

    vector<vector<array<int, 5>>> turns(H, vector<array<int, 5>>(W));
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) turns[y][x].fill(INF_INT);
    deque<tuple<int, int, int>> dq;
    turns[si][sj][4] = 0;
    dq.emplace_back(si, sj, 4);

    while (!dq.empty()) {
        auto [y, x, dir] = dq.front();
        dq.pop_front();
        int cur = turns[y][x][dir];
        for (int nd = 0; nd < 4; ++nd) {
            int ny = y + dy[nd], nx = x + dx[nd];
            if (!inside(nx, ny, W, H) || b[ny][nx] == 'T') continue;
            if (dist[ny][nx] != dist[y][x] + 1) continue; // 最短路DAGのみ
            int w  = (dir != 4 && dir != nd) ? 1 : 0;     // 方向が変われば+1
            int nt = cur + w;
            if (nt < turns[ny][nx][nd]) {
                turns[ny][nx][nd] = nt;
                if (w == 0) dq.emplace_front(ny, nx, nd);
                else dq.emplace_back(ny, nx, nd);
            }
        }
    }
    int ans = INF_INT;
    for (int d = 0; d < 4; ++d) ans = min(ans, turns[gi][gj][d]);
    return ans;
}

struct Cost {
    int dist, turns;
};

Cost shortest_cost_dag(const vector<vector<char>> &b, int si, int sj, int gi, int gj,
                       vector<vector<int>> *distS_out = nullptr) {
    auto distS = compute_dist_from(b, si, sj);
    int d      = distS[gi][gj];
    if (d >= INF_INT) return {INF_INT, INF_INT};
    int t = min_turns_on_shortest_paths(b, distS, si, sj, gi, gj);
    if (distS_out) *distS_out = std::move(distS);
    return {d, t};
}

// ========== deg / leaves 構築 & 差分適用 ==========
void build_degrees_and_leaves(const vector<vector<char>> &b, vector<vector<int>> &deg, int &leaves) {
    const int H = (int)b.size(), W = (int)b[0].size();
    deg.assign(H, vector<int>(W, 0));
    leaves = 0;
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            if (b[y][x] == 'T') continue;
            int c = 0;
            for (int d = 0; d < 4; ++d) {
                int ny = y + dy[d], nx = x + dx[d];
                if (inside(nx, ny, W, H) && b[ny][nx] != 'T') ++c;
            }
            deg[y][x] = c;
            if (c == 1) ++leaves;
        }
}

struct CloseRecord {
    int y, x;
    int self_prev_deg;
    bool self_prev_open;
    int leaf_delta;
    vector<tuple<int, int, int>> neigh_prev_deg; // (ny,nx,prev_deg)
};

bool apply_close_with_record(vector<vector<char>> &b, vector<vector<int>> &deg, int &leaves, int y,
                             int x, CloseRecord &rec) {
    if (b[y][x] == 'T') {
        rec.self_prev_open = false;
        rec.leaf_delta     = 0;
        return false;
    }
    rec.y              = y;
    rec.x              = x;
    rec.self_prev_open = true;
    rec.self_prev_deg  = deg[y][x];
    rec.neigh_prev_deg.clear();
    int delta = 0;

    if (deg[y][x] == 1) --delta; // 自身が葉なら減る
    b[y][x] = 'T';               // 一旦閉じる

    const int H = (int)b.size(), W = (int)b[0].size();
    for (int d = 0; d < 4; ++d) {
        int ny = y + dy[d], nx = x + dx[d];
        if (!inside(nx, ny, W, H)) continue;
        if (b[ny][nx] == 'T') continue;
        int before = deg[ny][nx];
        rec.neigh_prev_deg.emplace_back(ny, nx, before);
        int after = before - 1;
        if (before == 1) --delta;      // 葉→孤立
        else if (before == 2) ++delta; // 2→1 で新しい葉
        deg[ny][nx] = after;
    }
    leaves += delta;
    rec.leaf_delta = delta;
    return true;
}

void revert_close_with_record(vector<vector<char>> &b, vector<vector<int>> &deg, int &leaves,
                              const CloseRecord &rec) {
    if (!rec.self_prev_open) return;
    leaves -= rec.leaf_delta;
    b[rec.y][rec.x] = '.';
    for (auto &t : rec.neigh_prev_deg) {
        int ny, nx, prev;
        tie(ny, nx, prev) = t;
        deg[ny][nx]       = prev;
    }
    deg[rec.y][rec.x] = rec.self_prev_deg;
}

// ========== 関節点（Tarjan） ==========
vector<vector<char>> compute_articulation_mask(const vector<vector<char>> &b, int si, int sj) {
    const int H = (int)b.size(), W = (int)b[0].size();
    vector<vector<char>> art(H, vector<char>(W, 0));
    if (!inside(sj, si, W, H) || b[si][sj] == 'T') return art;

    const int N = H * W;
    vector<int> disc(N, -1), low(N, -1), parent(N, -1);
    int T = 0;

    auto idx = [W](int y, int x) { return y * W + x; };
    auto YX  = [W](int id) { return pair<int, int>(id / W, id % W); };

    function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = ++T;
        auto [yu, xu]    = YX(u);
        int child        = 0;

        for (int d = 0; d < 4; ++d) {
            int ny = yu + dy[d], nx = xu + dx[d];
            if (!inside(nx, ny, W, H) || b[ny][nx] == 'T') continue;
            int v = idx(ny, nx);
            if (disc[v] == -1) {
                parent[v] = u;
                ++child;
                dfs(v);
                low[u] = min(low[u], low[v]);
                if (parent[u] == -1 && child > 1) art[yu][xu] = 1;         // root で子2つ以上
                if (parent[u] != -1 && low[v] >= disc[u]) art[yu][xu] = 1; // 一般条件
            } else if (v != parent[u]) {
                low[u] = min(low[u], disc[v]);
            }
        }
    };

    dfs(idx(si, sj));
    return art;
}

// ========== スコア ==========
struct Weights {
    double wD = 3.0, wT = 1.0, wL = 2.0, wW = 0.25;
};
inline double score_from_components(const Cost &c, int leaves, int placed, const Weights &W) {
    if (c.dist >= INF_INT) return -1e18;
    return W.wD * c.dist + W.wT * c.turns + W.wL * leaves - W.wW * placed;
}

// ========== 高速版 grow ==========
void grow_branches_minimal_fast(vector<vector<char>> &b, int si, int sj, int gi, int gj, int passes = 4,
                                uint64_t seed = 114514ULL, Weights W = Weights(),
                                int max_fail_rounds = 1) {
    const int H = (int)b.size(), Wd = (int)b[0].size();
    mt19937_64 rng(seed);

    // 候補（S/G以外の'.'）
    vector<pair<int, int>> cand;
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < Wd; ++x) {
            if (!(y == si && x == sj) && !(y == gi && x == gj) && b[y][x] != 'T')
                cand.emplace_back(y, x);
        }

    // 初期：deg/leaves、最短コスト、distS/distG、関節点
    vector<vector<int>> deg;
    int leaves = 0;
    build_degrees_and_leaves(b, deg, leaves);

    vector<vector<int>> distS = compute_dist_from(b, si, sj);
    vector<vector<int>> distG = compute_dist_from(b, gi, gj);
    Cost base_cost{distS[gi][gj], INF_INT};
    if (base_cost.dist < INF_INT) {
        base_cost.turns = min_turns_on_shortest_paths(b, distS, si, sj, gi, gj);
    }
    double best = score_from_components(base_cost, leaves, /*placed=*/0, W);
    if (best < -1e17) return;

    auto art = compute_articulation_mask(b, si, sj);

    int placed = 0, fail_rounds = 0;
    for (int p = 0; p < passes; ++p) {
        shuffle(cand.begin(), cand.end(), rng);
        bool progressed = false;

        for (auto [y, x] : cand) {
            if (b[y][x] == 'T') continue;

            int dg = deg[y][x];
            if (dg <= 1) continue;   // 葉や孤立近傍は閉じない
            if (art[y][x]) continue; // 関節点は閉じない

            if (dg == 2) {
                bool near_junc = false;
                for (int d = 0; d < 4; ++d) {
                    int ny = y + dy[d], nx = x + dx[d];
                    if (inside(nx, ny, Wd, H) && b[ny][nx] != 'T' && deg[ny][nx] >= 3) near_junc = true;
                }
                if (!near_junc) continue;
            }

            CloseRecord rec;
            if (!apply_close_with_record(b, deg, leaves, y, x, rec)) continue;

            bool on_sp = on_any_shortest_path(distS, distG, gi, gj, y, x);

            Cost c1 = base_cost;
            if (on_sp) {
                auto tmpDist = compute_dist_from(b, si, sj);
                int d        = tmpDist[gi][gj];
                if (d >= INF_INT) {
                    revert_close_with_record(b, deg, leaves, rec);
                    continue; // S→G断絶は即却下
                }
                int t = min_turns_on_shortest_paths(b, tmpDist, si, sj, gi, gj);
                c1    = {d, t};
            }

            double sc = score_from_components(c1, leaves, placed + 1, W);
            if (sc > best) {
                best = sc;
                ++placed;
                progressed = true;
                base_cost  = c1;

                // 採用時のみ重い更新
                distS = compute_dist_from(b, si, sj);
                distG = compute_dist_from(b, gi, gj);
                art   = compute_articulation_mask(b, si, sj);
            } else {
                revert_close_with_record(b, deg, leaves, rec);
            }
        }

        if (!progressed) {
            if (++fail_rounds > max_fail_rounds) break;
        } else {
            fail_rounds = 0;
        }
    }
}

// ========== メイン ==========
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    int gi, gj; // 伝説の花の位置 (行,列)
    if (!(cin >> N >> gi >> gj)) return 0;
    int si = 0, sj = N / 2; // 入口 (行,列) = (0, floor(N/2))

    vector<vector<char>> b(N, vector<char>(N));
    rep(y, N) rep(x, N) cin >> b[y][x];
    auto origin = b;

    // (1) G の入口を可能なら 1 本に絞る（S→Gは必ず残す）
    narrow_goal(b, si, sj, gi, gj);

    // (2) 枝を増やしつつ最小限の壁だけ追加（連結厳守）
    Weights W;
    W.wD = 3.0;  // 最短距離の重み
    W.wT = 1.0;  // 曲がり回数の重み
    W.wL = 2.0;  // 葉(枝先)の重み
    W.wW = 0.25; // 壁ペナ
    grow_branches_minimal_fast(b, si, sj, gi, gj,
                               /*passes=*/5, /*seed=*/20250921ULL, W, /*max_fail_rounds=*/1);

    // 変更差分（'.'→'T'）
    vector<pair<int, int>> ans;
    rep(y, N) rep(x, N) if (b[y][x] != origin[y][x] && b[y][x] == 'T') ans.emplace_back(y, x);

    bool first = true;
    while (true) {
        ll py, px; // 現在位置 (行,列)
        if (!(cin >> py >> px)) break;
        if (py == gi && px == gj) break;

        ll n;
        if (!(cin >> n)) break;
        vector<ll> xs(n), ys(n);
        rep(i, n) cin >> ys[i] >> xs[i];

        if (first) {
            first = false;
            cout << (int)ans.size();
            for (auto [y, x] : ans) cout << ' ' << y << ' ' << x;
            cout << el << flush;
        } else {
            cout << 0 << el; // （インタラクティブ要件がなければ flush は不要）
        }
    }
    return 0;
}
