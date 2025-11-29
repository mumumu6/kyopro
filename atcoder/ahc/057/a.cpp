#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
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

template <class T> using P = pair<T, T>;
#define x first
#define y second
template <class T> P<T> operator+(const P<T> &a, const P<T> &b) { return {a.x + b.x, a.y + b.y}; }
template <class T> P<T> operator-(const P<T> &a, const P<T> &b) { return {a.x - b.x, a.y - b.y}; }
template <class T> P<T> operator-(const P<T> &a) { return {-a.x, -a.y}; }
template <class T, class U> P<T> operator*(const P<T> &a, const U &b) { return {a.x * b, a.y * b}; }
template <class T, class U> P<T> operator/(const P<T> &a, const U &b) { return {a.x / b, a.y / b}; }
template <class T> P<T> &operator+=(P<T> &a, const P<T> &b) { return a = a + b; }
template <class T> P<T> &operator-=(P<T> &a, const P<T> &b) { return a = a - b; }
template <class T, class U> P<T> &operator*=(P<T> &a, const U &b) { return a = a * b; }
template <class T, class U> P<T> &operator/=(P<T> &a, const U &b) { return a = a / b; }
template <class T> P<T> rotate(const P<T> &a) { return {-a.y, a.x}; } // 90 degree ccw
template <class T> T dot(const P<T> &a, const P<T> &b) { return a.x * b.x + a.y * b.y; }
template <class T> T cross(const P<T> &a, const P<T> &b) { return dot(rotate(a), b); }
template <class T> T square(const P<T> &a) { return dot(a, a); }
template <class T> ld abs(const P<T> &a) { return hypotl(a.x, a.y); }
template <class T> T gcd(const P<T> &a) { return gcd(a.x, a.y); }
template <class T> P<T> normalize(P<T> a) {
    if (a == P<T>{}) return a;
    a /= gcd(a);
    if (a < P<T>{}) a = -a;
    return a;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, t, m, k, l;
    cin >> n >> t >> m >> k >> l;

    auto torus_dist2 = [&](pll a, pll b) {
        ll dx = abs(a.x - b.x);
        dx    = min(dx, l - dx); // トーラス
        ll dy = abs(a.y - b.y);
        dy    = min(dy, l - dy); // トーラス
        return dx * dx + dy * dy;
    };


    vector<pll> p(n);
    vector<pll> v(n);
    rep(i, n) { cin >> p[i].x >> p[i].y >> v[i].x >> v[i].y; }
    // --- seed を 10 個選ぶ ---
    vector<int> seeds;
    vector<bool> is_seed(n, false);
    seeds.push_back(0);
    is_seed[0] = true;

    while ((int)seeds.size() < m) { // m = 10
        ll best_d  = -1;
        int best_i = -1;
        rep(i, n) {
            if (is_seed[i]) continue;
            ll mind = INF;
            for (int s : seeds) { chmin(mind, torus_dist2(p[i], p[s])); }
            if (mind > best_d) {
                best_d = mind;
                best_i = i;
            }
        }
        seeds.push_back(best_i);
        is_seed[best_i] = true;
    }

    // 一旦自国0で全部結合させる
    dsu uf(n);

    vector<pair<ld, pll>> edges;
    vector<vector<pll>> dist(n);
    rep(i, n) reps(j, i + 1, n) {
        edges.push_back({torus_dist2(p[i], p[j]), {i, j}});
        dist[i].push_back({torus_dist2(p[i], p[j]), j});
        dist[j].push_back({torus_dist2(p[i], p[j]), i});
    }

    so(edges);

    vector<pll> ans;

    for (int seed : seeds) {
        so(dist[seed]);

        for (auto [_, u] : dist[seed]) {
            if (uf.size(seed) >= k) break;
            if (uf.size(u) > 1) continue;
            if (uf.same(seed, u)) continue;
            if (is_seed[u]) continue;
            ans.push_back({seed, u});
            uf.merge(seed, u);
        }
    }

    for (auto [u, w] : ans) cout << 0 << " " << u << " " << w << el;
}