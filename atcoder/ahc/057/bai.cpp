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

    // 前計算：全点間距離の二乗と、各点からの昇順近傍リスト
    vector<vector<ll>> dist2(n, vector<ll>(n, 0));
    vector<vector<pll>> dist(n);
    rep(i, n) {
        dist[i].reserve(n - 1);
        rep(j, n) {
            dist2[i][j] = torus_dist2(p[i], p[j]);
            if (i == j) continue;
            dist[i].push_back({dist2[i][j], j});
        }
        so(dist[i]);
    }

    // 時間いっぱい seed を探す
    auto clock_start = chrono::steady_clock::now();
    auto deadline    = clock_start + chrono::milliseconds(1900); // 2sec ぎりぎりまで使う
    mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

    auto build_seeds = [&](int first) {
        vector<int> seeds;
        seeds.reserve(m);
        vector<char> used(n, false);
        seeds.push_back(first);
        used[first] = true;

        while ((int)seeds.size() < m) {
            ll best_d  = -1;
            int best_i = -1;
            rep(i, n) {
                if (used[i]) continue;
                ll mind = INF;
                for (int s : seeds) { chmin(mind, dist2[i][s]); }
                if (mind > best_d) {
                    best_d = mind;
                    best_i = i;
                }
            }
            seeds.push_back(best_i);
            used[best_i] = true;
        }
        return seeds;
    };

    auto build_plan = [&](const vector<int> &order) {
        vector<char> is_seed(n, false);
        for (int s : order) is_seed[s] = true;

        dsu uf(n);
        vector<pll> edges;
        edges.reserve(n - m);
        ld cost = 0;

        for (int seed : order) {
            for (auto [_, u] : dist[seed]) {
                int rseed = uf.leader(seed);
                if (uf.size(rseed) >= k) break;
                if (uf.size(u) > 1) continue;
                if (uf.same(rseed, u) || is_seed[u]) continue;

                ll best_d  = INF;
                int best_v = -1;
                for (auto &comp : uf.groups()) {
                    if (uf.leader(comp[0]) != rseed) continue;
                    for (int vtx : comp) {
                        ll d2 = dist2[vtx][u];
                        if (d2 < best_d) {
                            best_d = d2;
                            best_v = vtx;
                        }
                    }
                    break; // seed 成分は1つだけ
                }
                if (best_v == -1) continue;

                edges.push_back({best_v, u});
                cost += sqrt(best_d);
                uf.merge(best_v, u);
            }
            if (uf.size(uf.leader(seed)) < k) return make_pair(INF, vector<pll>{});
        }

        if ((int)edges.size() != n - m) return make_pair(INF, vector<pll>{});
        return make_pair((ll)cost, edges);
    };

    auto seeds0               = build_seeds(0);
    auto [best_cost, bestans] = build_plan(seeds0);

    // 初期 seed を変えたり順序をシャッフルしたりして探索
    while (chrono::steady_clock::now() < deadline) {
        int first  = rng() % n;
        auto seeds = build_seeds(first);

        auto [c1, e1] = build_plan(seeds);
        if (c1 < best_cost) {
            best_cost = c1;
            bestans   = std::move(e1);
        }

        if (chrono::steady_clock::now() >= deadline) break;

        shuffle(seeds.begin(), seeds.end(), rng);
        auto [c2, e2] = build_plan(seeds);
        if (c2 < best_cost) {
            best_cost = c2;
            bestans   = std::move(e2);
        }
    }

    for (auto [u, w] : bestans) cout << 0 << " " << u << " " << w << el;
}

