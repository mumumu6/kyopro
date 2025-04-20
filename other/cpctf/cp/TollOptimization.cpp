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

    ll n, m, k;
    cin >> n >> m >> k;
    vector<ll> cc(m);
    rep(i, m) cin >> cc[i];

    vector<vector<pll>> g(n);

    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--, b--;
        g[a].push_back({b, cc[i]});
        g[b].push_back({a, cc[i]});
    }


    vector<vector<ll>> dist(n, vector<ll>(k + 1, 4e18));

    // cost , node , tiket
    priority_queue<tuple<ll, ll, ll>, vector<tuple<ll, ll, ll>>, greater<tuple<ll, ll, ll>>> pq;

    dist[0][0] = 0;
    pq.push({0, 0, 0});

    while (!pq.empty()) {
        auto [cost, node, tiket] = pq.top();
        pq.pop();

        if (dist[node][tiket] < cost) continue;

        for (auto [to, c] : g[node]) {

            if (tiket < k && dist[to][tiket + 1] > cost) {
                pq.push({cost, to, tiket + 1});
                dist[to][tiket + 1] = cost;
                // cout << "to : " << to << " cost : " << cost << " tiket : " << tiket + 1 << endl;
                // cout << "dist[to][tiket + 1] : " << dist[to][tiket + 1] << endl;
            }

            if (dist[to][tiket] > cost + c) {
                pq.push({cost + c, to, tiket});
                dist[to][tiket] = cost + c;
                // cout << "to : " << to << " cost : " << cost + c << " tiket : " << tiket << " c : " << c
                //      << endl;
                // cout << "dist[to][tiket] : " << dist[to][tiket] << endl;
            }
        }
    }

    ll min_dist = 4e18;
    rep(i, k + 1) chmin(min_dist, dist[n - 1][i]);

    if (min_dist == 4e18) cout << -1 << endl;
    else cout << min_dist << endl;
}