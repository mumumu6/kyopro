#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld     = long double;
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
#define x first
#define y second
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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n, k;
    ll t;
    if (!(cin >> n >> k >> t)) return 0;

    vector<string> right_wall(n);
    rep(i, n) cin >> right_wall[i];
    vector<string> down_wall(max(0, n - 1));
    rep(i, n - 1) cin >> down_wall[i];

    vector<pll> points(k);
    rep(i, k) cin >> points[i].first >> points[i].second;

    auto to_index = [&](int x, int y) { return x * n + y; };

    vector<char> moves;
    vector<pll> positions;
    positions.reserve(k * n * n);
    positions.push_back(points[0]);

    auto append_path = [&](pll s, pll g) {
        if (s == g) return;
        vector<int> prev(n * n, -1);
        vector<char> move_dir(n * n, '?');
        queue<int> que;
        int start   = to_index(s.first, s.second);
        int goal    = to_index(g.first, g.second);
        prev[start] = start;
        que.push(start);

        auto enqueue = [&](int nx, int ny, int v, char dir) {
            if (!inside(nx, ny, n, n)) return;
            int ni = to_index(nx, ny);
            if (prev[ni] != -1) return;
            prev[ni]     = v;
            move_dir[ni] = dir;
            que.push(ni);
        };

        while (!que.empty() && prev[goal] == -1) {
            int v = que.front();
            que.pop();
            int x = v / n;
            int y = v % n;
            if (y + 1 < n && right_wall[x][y] == '0') enqueue(x, y + 1, v, 'R');
            if (y - 1 >= 0 && right_wall[x][y - 1] == '0') enqueue(x, y - 1, v, 'L');
            if (x + 1 < n && down_wall[x][y] == '0') enqueue(x + 1, y, v, 'D');
            if (x - 1 >= 0 && down_wall[x - 1][y] == '0') enqueue(x - 1, y, v, 'U');
        }
        if (prev[goal] == -1) return;

        vector<char> seq;
        int cur = goal;
        while (cur != start) {
            seq.push_back(move_dir[cur]);
            cur = prev[cur];
        }
        reverse(all(seq));
        for (char d : seq) {
            moves.push_back(d);
            auto [x, y] = positions.back();
            if (d == 'U') --x;
            if (d == 'D') ++x;
            if (d == 'L') --y;
            if (d == 'R') ++y;
            positions.emplace_back(x, y);
        }
    };

    rep(i, k - 1) append_path(points[i], points[i + 1]);

    int L = moves.size();

    auto dir_to_idx = [&](char c) {
        if (c == 'R') return 0;
        if (c == 'D') return 1;
        if (c == 'L') return 2;
        return 3;
    };

    struct NodeStage {
        char dir;
        int cell_next;   // stage id for next visit to the same cell
        int global_next; // stage id for the next overall step
    };
    vector<NodeStage> nodes(1, NodeStage{'?', 0, 0});
    unordered_map<long long, int> key2node;
    auto pack_key = [&](int dir_idx, int cell_next, int global_next) -> long long {
        return (((long long)dir_idx << 42) | ((long long)cell_next << 21) | (long long)global_next);
    };

    vector<int> stage_of_step(L, 0);
    vector<int> next_stage_same_cell(n * n, 0);
    for (int i = L - 1; i >= 0; --i) {
        int cell        = to_index(positions[i].first, positions[i].second);
        int dir_idx     = dir_to_idx(moves[i]);
        int cell_next   = next_stage_same_cell[cell];
        int global_next = (i + 1 < L ? stage_of_step[i + 1] : 0);
        long long key   = pack_key(dir_idx, cell_next, global_next);
        int node_id;
        auto it = key2node.find(key);
        if (it == key2node.end()) {
            node_id = nodes.size();
            nodes.push_back(NodeStage{moves[i], cell_next, global_next});
            key2node[key] = node_id;
        } else {
            node_id = it->second;
        }
        stage_of_step[i]           = node_id;
        next_stage_same_cell[cell] = node_id;
    }
    vector<int> initial_node = next_stage_same_cell;
    

    int node_cnt     = nodes.size();
    auto choose_pair = [&](int total) -> pair<int, int> {
        int bestC   = total;
        int bestQ   = total;
        int bestSum = bestC + bestQ;

        int total_root = (int)sqrt(total);

        vector<int> dc = {0, 0, 1, 1};
        vector<int> dq = {0, 1, 0, 1};

        rep(i, 4) {
            int c = total_root + dc[i];
            int q = total_root + dq[i];

            if (c <= 0 || q <= 0) continue;
            if (c * q < total) continue;

            if (chmin(bestSum, c + q)) {
                bestC = c;
                bestQ = q;
            }
        }

        return {bestC, bestQ};
    };

    int start_node = stage_of_step[0];
    vector<int> order;
    order.push_back(start_node);
    rep(i, node_cnt) if (i != start_node) order.push_back(i);
    vector<int> rank(node_cnt);
    rep(i, node_cnt) rank[order[i]] = i;

    auto [C, Q] = choose_pair(node_cnt);
    vector<int> node_color(node_cnt), node_state(node_cnt);
    rep(i, node_cnt) {
        node_color[i] = rank[i] % C;
        node_state[i] = rank[i] / C;
    }
    int color_cnt = 0, state_cnt = 0;

    rep(i, node_cnt) {
        color_cnt = max(color_cnt, node_color[i] + 1);
        state_cnt = max(state_cnt, node_state[i] + 1);
    }

    vector<int> init_color(n * n, 0);
    rep(cell, n * n) init_color[cell] = node_color[initial_node[cell]];

    int M = node_cnt - 1;
    cout << color_cnt << spa << state_cnt << spa << M << el;
    rep(i, n) {
        rep(j, n) {
            if (j) cout << spa;
            cout << init_color[to_index(i, j)];
        }
        cout << el;
    }

    for (int id = 1; id < node_cnt; ++id) {
        auto &nd  = nodes[id];
        int paint = node_color[nd.cell_next];
        int nxts  = node_state[nd.global_next];
        cout << node_color[id] << spa << node_state[id] << spa << paint << spa << nxts << spa << nd.dir
             << el;
    }
}
