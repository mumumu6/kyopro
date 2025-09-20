#include <bits/stdc++.h>
using namespace std;

struct MazeOptions {
    int trials    = 500;              // how many random DFS attempts
    int maxPivot  = 6;                // at each node, how many times we reshuffle neighbours
    unsigned seed = std::random_device{}();
};

struct Pos {
    int i, j;
};

static inline bool inside(int i, int j, int N) { return 0 <= i && i < N && 0 <= j && j < N; }

// map (i,j) <-> index helpers -------------------------------------------------
static inline int pack(int i, int j, int N) { return i * N + j; }
static inline Pos unpack(int idx, int N) { return {idx / N, idx % N}; }

// BFS distance from every cell to goal (used as heuristic)
vector<int> bfs_dist_to_goal(const vector<string> &grid, Pos goal) {
    int N         = (int)grid.size();
    int total     = N * N;
    const int INF = 1e9;
    vector<int> dist(total, INF);
    queue<int> q;
    if (grid[goal.i][goal.j] == '.') {
        int gidx = pack(goal.i, goal.j, N);
        dist[gidx] = 0;
        q.push(gidx);
    }
    static const int di[4] = {-1, 1, 0, 0};
    static const int dj[4] = {0, 0, -1, 1};
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        Pos p = unpack(v, N);
        for (int d = 0; d < 4; ++d) {
            int ni = p.i + di[d], nj = p.j + dj[d];
            if (!inside(ni, nj, N)) continue;
            if (grid[ni][nj] != '.') continue;
            int to = pack(ni, nj, N);
            if (dist[to] > dist[v] + 1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }
    return dist;
}

struct NodeState {
    array<int, 4> order{}; // neighbour indices (flattened), -1 if none
    int count  = 0;        // usable entries in order
    int cursor = 0;        // next candidate index to try
    int pivot  = 0;        // number of reshuffles used
};

vector<int> random_long_path(mt19937_64 &rng, const vector<vector<int>> &adj,
                             const vector<int> &distGoal, int start, int goal,
                             int N, const MazeOptions &opt) {
    int total = N * N;
    vector<char> visited(total, 0);
    vector<int> bestIndices;
    vector<int> stack;
    vector<NodeState> states;
    stack.reserve(total);
    states.reserve(total);

    auto setup_state = [&](int idx, int vertex) {
        NodeState st;
        array<pair<double, int>, 4> cand{};
        int cc = 0;
        for (int to : adj[vertex]) {
            if (visited[to]) continue;
            int frontier = 0;
            for (int nn : adj[to]) if (!visited[nn]) ++frontier;
            double h = distGoal[to] >= 1e9 ? -5.0 : -(double)distGoal[to];
            double score = h + frontier * 0.7 + std::uniform_real_distribution<double>(0.0, 1.0)(rng);
            cand[cc++] = {score, to};
        }
        if (cc) {
            sort(cand.begin(), cand.begin() + cc,
                 [&](auto &a, auto &b) { return a.first > b.first; });
            for (int k = 0; k < cc; ++k) st.order[k] = cand[k].second;
        } else {
            st.order.fill(-1);
        }
        st.count = cc;
        st.cursor = 0;
        st.pivot  = 0;
        if ((int)states.size() <= idx) states.resize(idx + 1);
        states[idx] = st;
    };

    stack.push_back(start);
    visited[start] = 1;
    setup_state(0, start);

    while (!stack.empty()) {
        int depth = (int)stack.size() - 1;
        int v     = stack.back();
        if (v == goal) {
            if (stack.size() > bestIndices.size()) bestIndices = stack;
        }
        NodeState &st = states[depth];
        bool advanced = false;
        while (st.cursor < st.count) {
            int to = st.order[st.cursor++];
            if (visited[to]) continue;
            visited[to] = 1;
            stack.push_back(to);
            setup_state((int)stack.size() - 1, to);
            advanced = true;
            break;
        }
        if (advanced) continue;

        if (st.pivot < opt.maxPivot && st.count > 1) {
            ++st.pivot;
            shuffle(st.order.begin(), st.order.begin() + st.count, rng);
            st.cursor = 0;
            continue;
        }

        visited[v] = 0;
        stack.pop_back();
    }
    return bestIndices;
}

vector<int> bfs_shortest_path(const vector<vector<int>> &adj, int start, int goal, int N) {
    int total = N * N;
    vector<int> dist(total, -1), prev(total, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (v == goal) break;
        for (int to : adj[v]) {
            if (dist[to] != -1) continue;
            dist[to]  = dist[v] + 1;
            prev[to]  = v;
            q.push(to);
        }
    }
    vector<int> path;
    if (dist[goal] == -1) return path;
    for (int cur = goal; cur != -1; cur = prev[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    return path;
}

vector<string> make_long_single_path(const vector<string> &grid, int ti, int tj,
                                     MazeOptions opt = {}) {
    int N    = (int)grid.size();
    int Sidx = pack(0, N / 2, N);
    int Gidx = pack(ti, tj, N);

    vector<vector<int>> adj(N * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] != '.') continue;
            int v = pack(i, j, N);
            static const int di[4] = {-1, 1, 0, 0};
            static const int dj[4] = {0, 0, -1, 1};
            for (int d = 0; d < 4; ++d) {
                int ni = i + di[d], nj = j + dj[d];
                if (!inside(ni, nj, N)) continue;
                if (grid[ni][nj] != '.') continue;
                adj[v].push_back(pack(ni, nj, N));
            }
        }
    }
    auto distGoal = bfs_dist_to_goal(grid, {ti, tj});

    mt19937_64 rng(opt.seed);
    vector<int> bestIndices;
    for (int tr = 0; tr < opt.trials; ++tr) {
        auto attempt = random_long_path(rng, adj, distGoal, Sidx, Gidx, N, opt);
        if (attempt.empty()) continue;
        if (attempt.back() != Gidx) continue; // must reach goal
        if (attempt.size() > bestIndices.size()) bestIndices = std::move(attempt);
    }

    if (bestIndices.empty() || bestIndices.back() != Gidx) {
        bestIndices = bfs_shortest_path(adj, Sidx, Gidx, N);
        if (bestIndices.empty()) return grid; // fallback: impossible case
    }

    auto key = [&](int idx) -> long long {
        Pos p = unpack(idx, N);
        return (long long)p.i << 32 | (unsigned)p.j;
    };

    unordered_set<long long> keep;
    keep.reserve(bestIndices.size() * 2);
    for (int v : bestIndices) keep.insert(key(v));

    vector<string> out = grid;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] != '.') continue;
            if (!keep.count((long long)i << 32 | (unsigned)j)) out[i][j] = 'T';
        }
    }
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    int ti, tj;
    cin >> ti >> tj;
    vector<string> grid(N);
    for (int i = 0; i < N; ++i) cin >> grid[i];

    MazeOptions opt;
    opt.trials   = 600;
    opt.maxPivot = 6;

    auto maze = make_long_single_path(grid, ti, tj, opt);
    for (auto &row : maze) cout << row << '\n';
    return 0;
}
