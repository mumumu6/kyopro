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
#define el endl;

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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n;
    cin >> n;
    vecc g(n);

    rep(i, n -1) {
        ll u, v;
        cin >> u >> v;
        u--;
        v--;

        g[u].pb(v);
        g[v].pb(u);
    }

    set<pll> edges;

    rep(i, n) rep(j, n) {
        if (i == j) continue;
        set<ll> seen;
        seen.insert(i);

        auto dfs = [&](auto dfs, ll s, ll to, set<ll> &seen) -> bool {
            if (s == to) {
                if (seen.size() % 2 == 0) return true; // 偶数個の頂点を訪問した場合
                else return false;                     // 奇数個の頂点を訪問した場合
            };

            for (auto v : g[s]) {
                if (seen.contains(v)) continue;
                seen.insert(v); // 訪問済みとしてマーク
                if (dfs(dfs, v, to, seen)) return true;
                seen.erase(v); // 戻るときに未訪問に戻す
            }
            return false;
        };

        if (dfs(dfs, i, j, seen)) { edges.insert({min(i, j), max(i, j)}); }
    }

    if (edges.size() % 2 == 0) {
        cout << "Second" << el;
    } else {
        cout << "First" << el;
        cout << edges.begin()->ft + 1 << spa << edges.begin()->sd + 1 << el; // 最小の辺を出力
        edges.erase(edges.begin());                                          // 最小の辺を削除
    }

    while (true) {
        ll u, v;
        cin >> u >> v;
        if (u == -1 && v == -1) {
            break; // 終了条件
        }
        u--;
        v--;

        edges.erase({min(u, v), max(u, v)}); // 入力された辺を削除

        cout << edges.begin()->ft + 1 << spa << edges.begin()->sd + 1 << el; // 最小の辺を出力
    }
}