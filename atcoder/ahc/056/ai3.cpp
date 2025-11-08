#include <bits/stdc++.h>
using namespace std;
//#include <atcoder/all>
//using namespace atcoder;
//using mint = modint998244353;
using ll = long long;
using ld = long double;
using i128 = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i,a,b) for(ll i = (b) - 1; i >= (a); i--)
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = { 0, 1, 1, 1, 0, -1, -1, -1 }, cos45 = { 1, 1, 0, -1, -1, -1, 0, 1 };
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0,-1, 0};
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define x first
#define y second
#define all(x) std::begin(x), std::end(x)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(),z.end())
#define sor(z) sort(z.rbegin(),z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n' 

static inline bool inside(int x,int y,int w,int h){
return 0<=x && x<w && 0<=y && y<h;
}

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

//template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
//   return os << m.val();
//}

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
    for(auto &[k, v] : m) {
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
    ll id = 0;
    auto out  = [&](auto &&v) {
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
        int start = to_index(s.first, s.second);
        int goal  = to_index(g.first, g.second);
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
    if ((ll)L > t) {
        moves.resize(t);
        positions.resize(min<ll>(positions.size(), t + 1));
        L = moves.size();
    }
    while ((int)positions.size() < L + 1) positions.push_back(positions.back());

    if (L == 0) {
        cout << 1 << spa << 1 << spa << 0 << el;
        rep(i, n) {
            rep(j, n) {
                if (j) cout << spa;
                cout << 0;
            }
            cout << el;
        }
        return 0;
    }

    vector<int> step_cell(L);
    rep(i, L) step_cell[i] = to_index(positions[i].first, positions[i].second);

    vector<int> next_visit_step(L, -1);
    vector<int> last_visit(n * n, -1);
    for (int step = L - 1; step >= 0; --step) {
        int cell = step_cell[step];
        next_visit_step[step] = last_visit[cell];
        last_visit[cell]      = step;
    }

    vector<int> first_visit(n * n, -1);
    rep(i, L) if (first_visit[step_cell[i]] == -1) first_visit[step_cell[i]] = i;

    auto dir_to_idx = [&](char c) {
        if (c == 'R') return 0;
        if (c == 'D') return 1;
        if (c == 'L') return 2;
        if (c == 'U') return 3;
        return 4; // 'S'
    };

    struct Key {
        int next_state;
        int next_color;
        int dir;
        bool operator==(const Key &other) const {
            return next_state == other.next_state && next_color == other.next_color && dir == other.dir;
        }
    };
    struct KeyHash {
        size_t operator()(const Key &k) const {
            uint64_t x = (uint64_t)k.next_state * 1000003ULL + (uint64_t)k.next_color;
            x = (x << 3) ^ (uint64_t)k.dir;
            return (size_t)x;
        }
    };

    struct NodeStage {
        char dir;
        int next_state;
        int next_color;
    };
    vector<NodeStage> nodes(1, NodeStage{'S', 0, 0});
    unordered_map<Key, int, KeyHash> trans_id;

    vector<int> stage_id(L, 0);
    for (int step = L - 1; step >= 0; --step) {
        int next_state = (step + 1 < L) ? stage_id[step + 1] : 0;
        int next_color = (next_visit_step[step] != -1) ? stage_id[next_visit_step[step]] : 0;
        int dir_idx    = dir_to_idx(moves[step]);
        Key key{next_state, next_color, dir_idx};
        auto it = trans_id.find(key);
        if (it == trans_id.end()) {
            int new_id = nodes.size();
            nodes.push_back(NodeStage{moves[step], next_state, next_color});
            trans_id[key] = new_id;
            stage_id[step] = new_id;
        } else {
            stage_id[step] = it->second;
        }
    }

    vector<int> initial_node(n * n, 0);
    rep(cell, n * n) {
        int first = first_visit[cell];
        if (first != -1) initial_node[cell] = stage_id[first];
    }

    int node_cnt = nodes.size();
    auto choose_pair = [&](int total) -> pair<int, int> {
        int limit = 100;
        int bestC = min(limit, total);
        int bestQ = (total + bestC - 1) / bestC;
        ll bestSum = (ll)bestC + bestQ;
        for (int C = 1; C <= min(limit, total); ++C) {
            int Q = (total + C - 1) / C;
            if (Q > limit) continue;
            if ((ll)C + Q < bestSum) {
                bestSum = C + Q;
                bestC   = C;
                bestQ   = Q;
            }
        }
        if ((ll)bestC * bestQ < total) {
            bestC = min(limit, total);
            bestQ = (total + bestC - 1) / bestC;
        }
        return {bestC, bestQ};
    };

    int start_node = stage_id[0];
    vector<int> order;
    vector<char> used(node_cnt, 0);
    auto push_order = [&](int id) {
        if (id < 0 || id >= node_cnt) return;
        if (used[id]) return;
        used[id] = 1;
        order.push_back(id);
    };
    push_order(start_node);
    push_order(0);
    rep(i, node_cnt) push_order(i);
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
        char d         = nodes[id].dir;
        int nxt_state  = nodes[id].next_state;
        int nxt_color  = nodes[id].next_color;
        cout << node_color[id] << spa << node_state[id] << spa << node_color[nxt_color] << spa << node_state[nxt_state] << spa << d << el;
    }
}
