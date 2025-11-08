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

bool is_prefix(const vector<char> &small, const vector<char> &large) {
    if (small.size() > large.size()) return false;
    return equal(small.begin(), small.end(), large.begin());
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, t;
    cin >> n >> k >> t;

    vector<vector<ll>> g(n * n);
    vector<pll> points(k);

    // xy座標から indexへ
    auto to_index   = [&](ll x, ll y) { return x * n + y; };
    auto to_index_p = [&](pll x) { return x.x * n + x.y; };

    auto get_dir = [&](int from, int to) -> char {
        int fx = from / n;
        int fy = from % n;
        int tx = to / n;
        int ty = to % n;

        if (fx == tx) {
            if (fy + 1 == ty) return 'R';
            if (fy - 1 == ty) return 'L';
        } else if (fy == ty) {
            if (fx + 1 == tx) return 'D';
            if (fx - 1 == tx) return 'U';
        }

        return '?';
    };
    auto dir_to_idx = [&](char c) {
        if (c == 'R') return 0;
        if (c == 'D') return 1;
        if (c == 'L') return 2;
        return 3;
    };

    rep(i, n) {
        string row;
        cin >> row;
        rep(j, n - 1) {
            int v = row[j] - '0';
            if (v) continue;

            g[to_index(i, j)].push_back(to_index(i, j + 1));
            g[to_index(i, j + 1)].push_back(to_index(i, j));
        }
    }

    rep(i, n - 1) {
        string row;
        cin >> row;
        rep(j, n) {
            int u = row[j] - '0';

            if (u) continue;

            g[to_index(i, j)].push_back(to_index(i + 1, j));
            g[to_index(i+1, j)].push_back(to_index(i, j));
        }
    }

    rep(i, k) {
        ll x, y;
        cin >> x >> y;
        points[i] = {x, y};
    }

    // 書くマスに対してどういう方向でそこを通るかを持つ

    vector<vector<char>> next_dir(n * n);
    vector<int> move;
    // 各マスにどういう方向で入るかをBFSで決定
    rep(i, k) {
        int start = to_index_p(points[i]);
        int goal  = to_index_p(points[(i + 1) % k]);

        queue<int> que;
        vector<int> prev(n * n, -1);

        que.push(start);
        prev[start] = start;

        while (!que.empty() && prev[goal] == -1) {
            int v = que.front();
            que.pop();

            for (auto nv : g[v]) {
                if (prev[nv] != -1) continue;
                prev[nv] = v;
                que.push(nv);
            }
        }

        int cur = goal;
        vector<int> path;
        while (cur != start) {
            int pv = prev[cur];
            next_dir[pv].push_back(get_dir(pv, cur));
            cur = pv;
            path.push_back(cur);
        }
        reverse(all(path));
        move.insert(move.end(), all(path));
    }

    rep(cell, n * n) { reverse(all(next_dir[cell])); }

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

    int node_cnt = move.size();
    auto [c, q]  = choose_pair(node_cnt);

    cout << c << spa << q << spa  <<  node_cnt - 1 << el;

    // color = node % c
    // state = node / c

    vector<int> init_color(n * n, -1);
    vector<queue<int>> stage_of_step(n*n);

    rep(i, move.size()) {
        int cell = move[i];

        stage_of_step[cell].push(i);
        if (init_color[cell] == -1) { init_color[cell] = i % c; }
    }
    rep(i, n * n) {
        if (init_color[i] == -1) init_color[i] = 0;
        stage_of_step[i].push(0); // dummy state
        stage_of_step[i].pop();
    }

    rep(i, n) {
        rep(j, n) { cout << init_color[to_index(i, j)] << spa; }
        cout << el;
    }

    rep(i, move.size() - 1) {
        int from       = move[i];
        int to         = move[i + 1];
        char d         = get_dir(from, to);
        
        int next_cell_stage = stage_of_step[from].front();
        stage_of_step[from].pop();
        int next_color = next_cell_stage % c;
        int next_state = (i + 1) / c;


        cout << i % c << spa << i / c << spa << next_color << spa << next_state << spa << d << el;
    }
}
