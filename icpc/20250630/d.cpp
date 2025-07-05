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
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
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

    ll n;
    cin >> n;

    vector<pair<ll, ll>> px; // x 座標とindexをもつ
    vector<pair<ll, ll>> py; // x 座標とindexをもつ
    vector<pll> p;
    rep(i, n) {
        ll a, b;
        cin >> a >> b;

        a--, b--;
        px.push_back({a, i});
        py.push_back({b, i});
        p.push_back({a, b});
    }

    so(px);
    so(py);

    vecc g(n);

    // debug(px);
    // debug(py);

    rep(i, n) {

        ll idx = px[i].sd;
        ll idy = py[i].sd;

        if (i + 1 < n) {
            g[idx].push_back(px[i + 1].sd);
            g[px[i + 1].sd].push_back(idx);

            ll t = i + 2;

        }

        if (i + 1 < n) {
            g[idy].push_back(py[i + 1].sd);
            g[py[i + 1].sd].push_back(idy);

            ll t = idy + 2;
            // debug(idy, py[i + 1].sd, p[idy], p[py[i + 1].sd]);
            // debug(idy, py[idy + 1].sd, p[idy], p[py[idy + 2].sd]);
            // cerr << "aaaaaaaaaaaaaaa " << el;

        }
    }

    // debug(g);

    priority_queue<pll, vector<pll>, greater<pll>> que; // costとidx
    vector<ll> dist(n, 4e18);
    dist[0] = 0;
    que.push({0, 0});

    while (que.size() > 0) {
        auto [cost, id] = que.top();
        que.pop();

        for (ll x : g[id]) {
            ll nc = min(abs(p[x].ft - p[id].ft), abs(p[x].sd - p[id].sd));

            if (dist[x] > cost + nc) {
                dist[x] = cost + nc;
                que.push({dist[x], x});

                // debug(dist[x], id, x, p[id], p[x], nc);
            }
        }
    }
    

    cout << dist[n - 1] << el;
}