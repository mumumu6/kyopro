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
#define vecc vector<vector<ll>>

struct edge {
    ll to;
    ll cost;
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n, m;
    cin >> n >> m;

    vector<vector<edge>> g(n, vector<edge>());
    rep(i, m) {
        ll a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        g[a].emplace_back(b, c);
        g[b].emplace_back(a, c);
    }

    vector<ll> dist(n, 4e18);

    priority_queue<pair<ll, ll>> q;
    q.push({0, 0});
    while (!q.empty()) {
        auto [idx, cost] = q.top();
        q.pop();

        if (dist[idx] < cost) continue;
        dist[idx] = cost;

        rep(i, g[idx].size()) {
            auto [to, c] = g[idx][i];

            if (dist[to] > dist[idx] + c) {
                dist[to] = dist[idx] + c;
                q.push({to, dist[to]});
            }
        }
    }

    vec dist2(n,4e18);
    priority_queue<pll> q2;
    q2.push({n-1,0});

    while(!q2.empty()){
        auto [idx,cost] = q2.top();
        q2.pop();

        if(dist2[idx] < cost )continue;
        dist2[idx] = cost;

        rep(i,g[idx].size()){
            auto [to,c] = g[idx][i];

            if(dist2[to] > dist2[idx] + c ){
                dist2[to] = dist2[idx] + c;
                q2.push({to,dist2[to]});
            }
        }
    }


    rep(i,n){
        cout << dist[i] + dist2[i] << endl;
    }
}