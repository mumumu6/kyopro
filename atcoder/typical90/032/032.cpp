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

    ll n;
    cin >> n;
    vecc a(n, vec(n));
    rep(i, n) rep(j, n) cin >> a[i][j];

    ll m;
    cin >> m;
    vector<vector<bool>> x(n, vector(n, false));
    rep(i, m) {
        ll u, v;
        cin >> u >> v;
        u--;
        v--;
        x[u][v] = true;
        x[v][u] = true;
    }
    const ll inf = 4e18;
    ll ans       = inf;

    vec per(n, 0);
    rep(i, n) per[i] = i;

    do {
        ll t=0;
        bool check = false;
        rep(i, n - 1) {
            if (x[per[i]][per[i + 1]]) check = true;
        }
        rep(i, n) t += a[per[i]][i];

        if (!check) ans = min(ans, t);
    } while (next_permutation(all(per)));

    cout << (ans == inf ? -1 : ans) << endl;
}