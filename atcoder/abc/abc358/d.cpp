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
#define pb(x) push_back(x)
// #define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    // cin.tie(nullptr);
    // ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n, m;
    cin >> n >> m;
    vec a(n), b(m);
    vector<bool> check(n, false);

    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    vec jum(n, -1);

    sort(all(b));
    sort(all(a));

    ll ans = 0;
    rep(i, m) {
        if (b[i] > a[n - 1]) {
            cout << -1 << endl;

            return 0;
        }
        auto it = lower_bound(all(a), b[i]) - a.begin();
        }

    cout << ans << endl;
    return 0;
}