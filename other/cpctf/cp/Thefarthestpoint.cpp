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

    ll n;
    cin >> n;

    vector<vector<pll>> g(n); // 修正: 隣接リストとして初期化

    rep(i, n - 1) { 
        ll u, v, w;
        cin >> u >> v >> w;
        u--, v--;

        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    vector<bool> visited(n, false); 

    auto dfs = [&](auto dfs, ll v, ll cost, ll &farthest_node, ll &max_cost) -> void {
        if (cost > max_cost) {
            max_cost      = cost;
            farthest_node = v;
        }
        for (auto [to, w] : g[v]) {
            if (visited[to]) continue;
            visited[to] = true;
            dfs(dfs, to, cost + w, farthest_node, max_cost);
        }
    };

    ll farthest_node = 0, max_cost = 0;
    visited[0] = true;
    dfs(dfs, 0, 0, farthest_node, max_cost);

    fill(visited.begin(), visited.end(), false);
    max_cost              = 0;
    visited[farthest_node] = true;
    dfs(dfs, farthest_node, 0, farthest_node, max_cost);

    cout << max_cost << endl;
}