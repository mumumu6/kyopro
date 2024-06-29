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

ll inf = 4e18;
int main() {
    ll n, m;
    cin >> n >> m;

    vector<ll> dp(1 << n, inf);

    dp[0] = 0;

    rep(i, m) {
        vector<ll> next = dp;

        ll a, b;
        cin >> a >> b;
        ll c = 0;
        rep(j, b) {
            ll key;
            cin >> key;
            c |= 1 << (key - 1);
        }

        rep(j, 1 << n) {
            // next[j] = min(dp[j], next[j]);
            next[c | j] = min(dp[j] + a, next[c | j]);
        }

        swap(dp, next);
    }

    if (dp[(1 << n) - 1] >= inf) {
        cout << -1 << endl;
        return 0;
    }

    cout << dp[(1 << n) - 1] << endl;
}