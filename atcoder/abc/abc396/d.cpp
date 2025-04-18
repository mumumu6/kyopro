#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m;
    cin >> n >> m;

    // 辺の情報を保持する構造
    vector<vector<pair<ll, ll>>> g(n);
    vector<tuple<ll, ll, ll>> edges(m);

    rep(i, m) {
        ll u, v, w;
        cin >> u >> v >> w;

        u--;
        v--;

        // 辺の情報を保存（頂点番号と辺のラベル）
        g[u].push_back({v, i});
        g[v].push_back({u, i});
        edges[i] = {u, v, w};
    }

    // 頂点1から頂点Nへの単純パスを探索し、XOR値の最小値を求める
    ll ans = 4e18;

    auto dfs = [&](auto dfs, ll v, vector<bool> visted, ll x) -> void {
        visted[v] = true;

        if(v == n-1){
            chmin(ans,x);
            return;
        }

        for(auto [to, edge_id] : g[v]){
            if(visted[to]) continue;

            auto [u, v, w] = edges[edge_id];

            // 辺のラベルをXORする
            dfs(dfs, to, visted, x ^ w);
        }
        
    };
    
    dfs(dfs, 0, vector<bool>(n, false), 0);

    cout << ans << endl;
    return 0;
}