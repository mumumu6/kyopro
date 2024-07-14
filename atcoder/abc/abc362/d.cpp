#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
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
#define vecc vector<vector<int>>

struct edge {
    ll to;   // 行き先
    ll cost; // 　コスト
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n,m;
    cin >> n >> m;
    vec a(n);
    rep(i, n) cin >> a[i];

    vector<vector<edge>> graph(n, vector<edge>(2));
    vector<ll> dist(n, 4e18);
    dist[0] = a[0];

    rep(i, m) {
        ll u, v, b;
        cin >> u >> v >> b;
        u--;
        v--;

        graph[u].emplace_back(v, b);
        graph[v].emplace_back(u, b);
    }

    priority_queue<pll, vector<pll>, greater<pll>> que;

    que.push({0, 0});

    while (!que.empty()) {
        pll c = que.top();
        que.pop();
        int d   = c.ft; // スタート地点から現在地までの距離
        int idx = c.sd; // 現在地のindex

        if (d > dist[idx]) continue;
        rep(i, graph[idx].size()) {
            edge e = graph[idx][i];
            if (dist[e.to] > dist[idx] + e.cost + a[e.to]) {
                dist[e.to] = dist[idx] + e.cost + a[e.to];
                que.push({dist[e.to], e.to});
            }
        }
    }

    reps(i, 1, n)  cout << dist[i] << " "; 
    cout << endl;
}