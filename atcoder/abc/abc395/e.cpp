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

    ll n, m, x;
    cin >> n >> m >> x;

    vector<vector<vector<ll>>> g(n, vector<vector<ll>>(2));

    rep(i, m) {
        ll u, v;
        cin >> u >> v;
        u--, v--;
        g[u][0].push_back(v);
        g[v][1].push_back(u);
    }

    vector<vector<ll>> dp(n, vector<ll>(2, 4e18));

    // node, cost , onOff, 0だとoff, 1だとon
    priority_queue<tuple<ll, ll, ll>, vector<tuple<ll, ll, ll>>, greater<tuple<ll, ll, ll>>> que;

    que.push(make_tuple(0, 0, 0));

    dp[0][0] = 0;

    while (!que.empty()) {
        auto [cost, node, onoff] = que.top();
        que.pop();

        if (dp[node][onoff] < cost) continue;

        for (auto to : g[node][onoff]) {
            if (dp[to][onoff] > cost + 1) {
                dp[to][onoff] = cost + 1;
                que.push(make_tuple(cost + 1, to, onoff));
            }
        }

        for (auto to : g[node][1 - onoff]) {
            if (dp[to][1 - onoff] > cost + x + 1) {
                dp[to][1 - onoff] = cost + x + 1;
                que.push(make_tuple(cost + x + 1, to, 1 - onoff));
            }
        }
    }

    cout << min(dp[n - 1][0], dp[n - 1][1]) << endl;
}