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

struct edge {
    ll to;
    ll cost;
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n, m;
    cin >> n >> m;

    vector<vector<edge>> g(n, vector<edge>());

    rep(i, m) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }

    priority_queue<pll> q;
    vector<ll> dist(n, 4e18);
    q.push({0, 0});
    dist[0] = 0;

    while (!q.empty()) {
        auto [to, cost] = q.top();
        q.pop();
        if (dist[to] < cost) continue;

        for (auto [tt, d] : g[to]) {
            if (dist[tt] > cost + d) {
                q.push({tt, cost + d});
                dist[tt] = cost + d;
            };
        }
    }

    priority_queue<pll> qq;
    vector<ll> dist2(n, 4e18);
    qq.push({n - 1, 0});
    dist2[n - 1] = 0;

    while (!qq.empty()) {
        auto [to, cost] = qq.top();
        qq.pop();
        if (dist2[to] < cost) continue;

        for (auto [tt, d] : g[to]) {
            if (dist2[tt] > cost + d) {
                qq.push({tt, cost + d});
                dist2[tt] = cost + d;
            };
        }
    }

    rep(i, n) { cout << dist[i] + dist2[i] << endl; }
}