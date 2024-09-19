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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> to(n);
    rep(i, n - 1) {
        int a, b;
        cin >> a >> b;
        a--, b--;

        to[a].pb(b);
        to[b].pb(a);
    }

    vector<int> vs(k);
    rep(i, k) cin >> vs[i], vs[i]--;

    vector<bool> selected(n, false);
    rep(i, k) selected[vs[i]] = true;

    vector<int> num(n, 0);
    auto dfs = [&](auto dfs, int v, int p = -1) -> void {
        if (selected[v]) num[v]++;
        for (int u : to[v]) {
            if (u == p) continue;
            dfs(dfs, u, v);
            num[v] += num[u];
        }
    };

    dfs(dfs, vs[0]);

    int ans = 0;
    rep(i, n) if (num[i] > 0) ans++;

    cout << ans << endl;
    return 0;
}