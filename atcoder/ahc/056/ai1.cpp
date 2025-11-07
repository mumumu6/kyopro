// 経路全部に状態を与えて最短経路でつなげるだけ

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
    vector<char> program;

    // BFS shortest path between two points and append the directions.
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
        if (prev[goal] == -1) return; // should not happen due to connectivity

        vector<char> seq;
        int cur = goal;
        while (cur != start) {
            seq.push_back(move_dir[cur]);
            cur = prev[cur];
        }
        reverse(all(seq));
        program.insert(program.end(), seq.begin(), seq.end());
    };

    rep(i, k - 1) append_path(points[i], points[i + 1]);

    if ((ll)program.size() > t) program.resize(t); // guard just in case

    int C = 1;
    int Q = max(1, (int)program.size() + 1);
    int M = (int)program.size();

    cout << C << spa << Q << spa << M << el;
    rep(i, n) {
        rep(j, n) {
            if (j) cout << spa;
            cout << 0;
        }
        cout << el;
    }
    rep(i, M) { cout << 0 << spa << i << spa << 0 << spa << i + 1 << spa << program[i] << el; }
}
