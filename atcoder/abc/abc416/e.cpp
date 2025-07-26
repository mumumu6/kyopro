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
        if (id < sizeof...(Ts)) cerr << ",";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

void warshall_floyd(vecc &G) {
    ll n = G.size();
    for (ll k = 0; k < n; ++k) {
        for (ll i = 0; i < n; ++i) {
            for (ll j = 0; j < n; ++j) { G[i][j] = min(G[i][j], G[i][k] + G[k][j]); }
        }
    }
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m;
    cin >> n >> m;

    vecc dist(n + 1, vector<ll>(n + 1, INF)); // 空港間の距離
    rep(i, n + 1) dist[i][i] = 0;             // 自分自身への距離は0
    rep(i, m) {
        ll a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        chmin(dist[a][b], c);
        chmin(dist[b][a], c);
    }

    ll k, td;
    cin >> k >> td;

    rep(i, k) {
        ll v;
        cin >> v;
        v--;
        chmin(dist[v][n], td); // 空港から仮想のノードへの距離
        chmin(dist[n][v], 0);  // 仮想のノードから
    }

    ll sum = 0;
    warshall_floyd(dist);
    rep(i, n) rep(j, n) {
        if (dist[i][j] != INF) sum += dist[i][j];
    }
    // cout << sum - k * td << el; // 初期状態の出力

    // debug(dist);
    ll q;
    cin >> q;

    rep(qi, q) {
        ll tt;
        cin >> tt;

        if (tt == 1) {
            ll x, y, t;
            cin >> x >> y >> t;
            x--;
            y--;
            debug(dist[x][y], dist[y][x], sum, t);

            if (!chmin(dist[x][y], t)) { continue; }
            if (dist[y][x] == INF) sum -= 2 * t; // 新たに辺が追加された場合は、合計距離に加算
            else sum -= 2 * dist[y][x] - 2 * t;  // 既存の辺が更新された場合は、合計距離から引く
            chmin(dist[y][x], t);
            // debug(dist[x][y], dist[y][x], sum, t);

            for (ll i = 0; i < n + 1; ++i) {
                for (ll j = 0; j < n + 1; ++j) {
                    ll tmp = dist[i][j];
                    if (tmp == INF) tmp = 0; // INFの場合は無視

                    ll dif = 0;

                    chmin(dist[i][j], dist[i][x] + dist[x][y] + dist[y][j]);

                    chmin(dist[i][j], dist[i][y] + dist[y][x] + dist[x][j]);

                    if (i == n || j == n) continue; // 仮想ノードとの距離は無視
                    sum = sum - tmp + dist[i][j];

                    // debug(i, j, dist[i][j],dist[i][x], dist[x][y], dist[y][j], tmp, sum);
                }
            }
        } else if (tt == 2) {
            ll x;
            k++;
            cin >> x;
            x--;

            dist[x][n] = td; // 空港から仮想のノ
            dist[n][x] = 0;  // 仮想のノードから空港への距離を0にする

            for (ll i = 0; i < n + 1; ++i) {
                for (ll j = 0; j < n + 1; ++j) {
                    ll tmp = dist[i][j];
                    if (tmp == INF) tmp = 0;
                    chmin(dist[i][j], dist[i][x] + dist[x][n] + dist[n][j]);

                    chmin(dist[i][j], dist[i][n] + dist[n][x] + dist[x][j]);

                    if (i == n || j == n) continue; // 仮想ノードとの距離は無視
                    sum = sum - tmp + dist[i][j];
                }
            }
        } else if (tt == 3) {

            ll ans = 0;

            rep(i, n) rep(j, n) {
                if (dist[i][j] != INF) { ans += dist[i][j]; }
            }

            cout << ans << el;
        }
    }
}