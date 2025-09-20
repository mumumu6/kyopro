#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define all(x) begin(x), end(x)
#define spa " "
#define el '\n'
#define ft first
#define sd second

vector<int> dx = {0, 1, 0, -1};
vector<int> dy = {1, 0, -1, 0};

struct MazePlan {
    vector<pll> path_cells;
    vector<pll> block_cells;
};

static mt19937_64 rng_maze(chrono::steady_clock::now().time_since_epoch().count());

MazePlan build_maze(const vector<vector<char>> &b, pll s, pll t) {
    MazePlan plan;
    ll N = b.size();

    vector<vector<int>> id(N, vector<int>(N, -1));
    vector<pll> rev;
    int idx = 0;
    rep(i, N) rep(j, N) if (b[i][j] == '.') {
        id[i][j] = idx++;
        rev.emplace_back(i, j);
    }
    if (idx == 0) return plan;

    auto get_id = [&](pll p) -> int {
        if (p.ft < 0 || p.ft >= N || p.sd < 0 || p.sd >= N) return -1;
        return id[p.ft][p.sd];
    };

    int S = get_id(s);
    int T = get_id(t);
    if (S == -1 || T == -1) return plan;

    vector<vector<int>> adj(idx);
    vector<int> dist_target(idx, 0), dist_start(idx, 0);
    rep(i, N) rep(j, N) {
        int v = id[i][j];
        if (v == -1) continue;
        dist_target[v] = abs(i - t.ft) + abs(j - t.sd);
        dist_start[v]  = abs(i - s.ft) + abs(j - s.sd);
        rep(dir, 4) {
            int ni = i + dx[dir];
            int nj = j + dy[dir];
            if (ni < 0 || nj < 0 || ni >= N || nj >= N) continue;
            int u = id[ni][nj];
            if (u == -1) continue;
            adj[v].push_back(u);
        }
    }

    vector<int> best_parent(idx, -1);
    int best_depth    = -1;
    double best_score = -1e100;

    vector<int> parent(idx), depth(idx);
    vector<char> visited(idx);
    vector<vector<int>> ord(idx);
    vector<pair<int, int>> st;

    auto prepare_order = [&](vector<vector<int>> &out) {
        out = adj;
        rep(v, idx) {
            auto &nbr = out[v];
            shuffle(nbr.begin(), nbr.end(), rng_maze);
            stable_sort(nbr.begin(), nbr.end(), [&](int a, int b_) {
                int ka = -dist_target[a] * 20 + dist_start[a] * 3 + (rng_maze() % 30);
                int kb = -dist_target[b_] * 20 + dist_start[b_] * 3 + (rng_maze() % 30);
                return ka > kb;
            });
        }
    };

    int iter = min<int>(900, max<int>(300, idx * 4));
    for (int it = 0; it < iter; ++it) {
        prepare_order(ord);
        fill(visited.begin(), visited.end(), 0);
        fill(depth.begin(), depth.end(), 0);
        fill(parent.begin(), parent.end(), -1);
        st.clear();

        visited[S] = 1;
        st.emplace_back(S, 0);

        while (!st.empty()) {
            auto &[v, nxt] = st.back();
            if (nxt < (int)ord[v].size()) {
                int to = ord[v][nxt++];
                if (visited[to]) continue;
                visited[to] = 1;
                parent[to]  = v;
                depth[to]   = depth[v] + 1;
                st.emplace_back(to, 0);
            } else {
                st.pop_back();
            }
        }

        if (!visited[T]) continue;
        double score = depth[T] * 900.0 + (rng_maze() % 1000);
        if (depth[T] >= idx - 1) score -= 1e7;
        if (depth[T] > best_depth || (depth[T] == best_depth && score > best_score)) {
            best_depth  = depth[T];
            best_score  = score;
            best_parent = parent;
        }
    }

    if (best_depth == -1) {
        queue<int> q;
        vector<int> dist(idx, -1), par(idx, -1);
        dist[S] = 0;
        q.push(S);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int to : adj[v]) {
                if (dist[to] != -1) continue;
                dist[to] = dist[v] + 1;
                par[to]  = v;
                q.push(to);
            }
        }
        best_parent = par;
    }

    vector<vector<int>> tree(idx);
    rep(v, idx) {
        int p = best_parent[v];
        if (p != -1) {
            tree[v].push_back(p);
            tree[p].push_back(v);
        }
    }

    vector<int> force(idx, 0);
    {
        int cur = T;
        while (cur != -1) {
            force[cur] = 1;
            if (cur == S) break;
            cur = best_parent[cur];
        }
    }

    vector<char> keep(idx, 1);
    vector<int> degree(idx, 0);
    rep(v, idx) for (int to : tree[v]) if (keep[to]) ++degree[v];

    vector<int> dist_path(idx, (int)1e9);
    queue<int> q;
    rep(v, idx) if (force[v]) {
        dist_path[v] = 0;
        q.push(v);
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int to : tree[v]) {
            if (dist_path[to] <= dist_path[v] + 1) continue;
            dist_path[to] = dist_path[v] + 1;
            q.push(to);
        }
    }

    vector<int> rand_bias(idx);
    rep(v, idx) rand_bias[v] = (int)(rng_maze() % 1000);

    auto recompute_degree = [&]() {
        fill(degree.begin(), degree.end(), 0);
        rep(v, idx) if (keep[v]) for (int to : tree[v]) if (keep[to]) ++degree[v];
    };

    auto add_chain = [&](int v) {
        vector<int> path;
        int cur = v;
        while (cur != -1 && !keep[cur]) {
            path.push_back(cur);
            cur = best_parent[cur];
        }
        for (int node : path) keep[node] = 1;
        recompute_degree();
    };

    auto can_remove = [&](int v) -> bool {
        pll pos = rev[v];
        for (int di = -1; di <= 0; ++di) {
            for (int dj = -1; dj <= 0; ++dj) {
                ll si = pos.ft + di;
                ll sj = pos.sd + dj;
                if (si < 0 || sj < 0 || si + 1 >= N || sj + 1 >= N) continue;
                bool has_original = false;
                int open_cnt      = 0;
                rep(ox, 2) rep(oy, 2) {
                    int ni = si + ox;
                    int nj = sj + oy;
                    if (b[ni][nj] == 'T') continue;
                    has_original = true;
                    int w        = id[ni][nj];
                    if (w == -1) continue;
                    if (keep[w] && w != v) ++open_cnt;
                }
                if (has_original && open_cnt == 0) return false;
            }
        }
        return true;
    };

    auto push_leaf = [&](int v, priority_queue<pair<int, int>> &pq) {
        if (!keep[v] || force[v]) return;
        if (degree[v] <= 1) {
            int score = dist_path[v] * 5000 + dist_target[v] * 15 + rand_bias[v];
            pq.emplace(score, v);
        }
    };

    priority_queue<pair<int, int>> pq;
    rep(v, idx) push_leaf(v, pq);

    int forced_cnt = accumulate(force.begin(), force.end(), 0);
    int keep_count = idx;
    int target_keep = max(forced_cnt + (int)(idx * 0.05), (int)(idx * 0.35));

    while (keep_count > target_keep && !pq.empty()) {
        auto [score, v] = pq.top();
        pq.pop();
        if (!keep[v] || force[v]) continue;
        if (degree[v] != 1) {
            push_leaf(v, pq);
            continue;
        }
        if (!can_remove(v)) continue;

        keep[v] = 0;
        --keep_count;
        for (int to : tree[v]) {
            if (!keep[to]) continue;
            --degree[to];
            push_leaf(to, pq);
        }
        degree[v] = 0;
    }

    bool changed = true;
    while (changed) {
        changed = false;
        rep(i, N - 1) rep(j, N - 1) {
            array<int, 4> cells = {-1, -1, -1, -1};
            int ptr = 0;
            rep(ox, 2) rep(oy, 2) {
                int ni = i + ox;
                int nj = j + oy;
                int w  = id[ni][nj];
                if (w == -1) continue;
                cells[ptr++] = w;
            }
            if (!ptr) continue;
            int kept_local = 0;
            rep(k, ptr) if (cells[k] != -1 && keep[cells[k]]) ++kept_local;
            if (kept_local) continue;

            int chosen = -1;
            int best   = -1e9;
            rep(k, ptr) {
                int w = cells[k];
                if (w == -1) continue;
                int val = -dist_path[w] * 200 + dist_target[w] * 10 + rand_bias[w];
                if (val > best) {
                    best   = val;
                    chosen = w;
                }
            }
            if (chosen == -1) continue;
            add_chain(chosen);
            keep_count = accumulate(keep.begin(), keep.end(), 0);
            changed    = true;
        }
    }

    rep(v, idx) {
        if (keep[v]) plan.path_cells.push_back(rev[v]);
        else plan.block_cells.push_back(rev[v]);
    }

    cerr << "[build-maze] idx=" << idx << " keep=" << plan.path_cells.size() << " block="
         << plan.block_cells.size() << el;
    return plan;
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    ll N;
    pll target;
    if (!(cin >> N >> target.ft >> target.sd)) return 0;

    vector<vector<char>> grid(N, vector<char>(N));
    rep(i, N) rep(j, N) cin >> grid[i][j];

    vector<vector<char>> confirmed(N, vector<char>(N, 0));
    vector<vector<char>> planted(N, vector<char>(N, 0));

    bool prepared = false;
    vector<pll> pending_blocks;

    while (true) {
        pll pos;
        if (!(cin >> pos.ft >> pos.sd)) break;
        if (pos == target) break;

        ll n;
        cin >> n;
        vector<pll> newly(n);
        rep(i, n) cin >> newly[i].ft >> newly[i].sd;

        auto mark = [&](pll cell) {
            if (cell.ft < 0 || cell.ft >= N || cell.sd < 0 || cell.sd >= N) return;
            confirmed[cell.ft][cell.sd] = 1;
        };
        mark(pos);
        rep(i, n) mark(newly[i]);

        if (!prepared) {
            MazePlan plan  = build_maze(grid, pos, target);
            pending_blocks = plan.block_cells;
            prepared       = true;
            cerr << "[maze] path=" << plan.path_cells.size() << " block=" << plan.block_cells.size()
                 << el;
        }

        vector<pll> out_cells;
        for (auto [x, y] : pending_blocks) {
            if (confirmed[x][y]) continue;
            if (planted[x][y]) continue;
            out_cells.emplace_back(x, y);
            planted[x][y] = 1;
            grid[x][y]    = 'T';
        }
        pending_blocks.clear();

        if (out_cells.empty()) {
            cout << 0 << el;
            cout.flush();
            continue;
        }

        cout << out_cells.size();
        for (auto [x, y] : out_cells) cout << spa << x << spa << y;
        cout << el;
        cout.flush();
    }
    return 0;
}
