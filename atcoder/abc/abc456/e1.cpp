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
#define veccc vector<vecc>
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

    ll t;
    cin >> t;

    rep(ti, t) {
        ll n, m;
        cin >> n >> m;

        vector<pll> edge;
        rep(i, m) {
            ll u, v;
            cin >> u >> v;
            u--, v--;
            edge.push_back({u, v});
        }

        ll w;
        cin >> w;

        vector<string> s(n);
        rep(i, n) cin >> s[i];

        // グラフ作成
        // g[wi][x] = 曜日 wi に都市 x から、次の曜日に行ける都市たち
        vector<vector<vector<ll>>> g(w, vector<vector<ll>>(n));

        rep(wi, w) {
            ll nwi = (wi + 1) % w;

            for (auto [u, v] : edge) {
                if (s[u][wi] == 'o' && s[v][nwi] == 'o') {
                    g[wi][u].push_back(v);
                }
                if (s[v][wi] == 'o' && s[u][nwi] == 'o') {
                    g[wi][v].push_back(u);
                }
            }

            // 同じ都市に留まる辺
            rep(i, n) {
                if (s[i][wi] == 'o' && s[i][nwi] == 'o') {
                    g[wi][i].push_back(i);
                }
            }
        }

        // indeg[wi][x] = 頂点 (x, wi) に入ってくる辺の数 トポロジカルソート
        vector<vector<ll>> indeg(w, vector<ll>(n, 0));

        rep(wi, w) {
            ll nwi = (wi + 1) % w;

            rep(x, n) {
                for (ll y : g[wi][x]) {
                    indeg[nwi][y]++;
                }
            }
        }

        queue<pll> que;
        ll total = 0;

        // 存在する頂点、つまり s[x][wi] == 'o' の頂点だけ見る
        rep(wi, w) {
            rep(x, n) {
                if (s[x][wi] == 'o') {
                    total++;

                    if (indeg[wi][x] == 0) {
                        que.push({x, wi});
                    }
                }
            }
        }

        ll removed = 0;

        while (!que.empty()) {
            auto [x, wi] = que.front();
            que.pop();

            removed++;

            ll nwi = (wi + 1) % w;

            for (ll y : g[wi][x]) {
                indeg[nwi][y]--;

                if (indeg[nwi][y] == 0) {
                    que.push({y, nwi});
                }
            }
        }

        // 全部消せなかったら、残った部分にサイクルがある
        if (removed < total) {
            Yes;
        } else {
            No;
        }
    }
}