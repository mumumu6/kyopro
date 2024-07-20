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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    cin >> n;
    vec a(2 * n);
    rep(i, 2 * n) cin >> a[i];
    vecc dp(2 * n, vec(2 * n, 4e18));

    rep(i, 2 * n - 1) dp[i][i + 1] = abs(a[i] - a[i + 1]);

    reps(i, 2, 2 * n) rep(j, 2 * n) {
        int l = j, r = j + i;
        if (j + i >= 2 * n) continue;

        reps(k, l, r) {
            dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
            // cout << dp[l][r] << " l " << l << " r " << r << endl;
        }
        dp[l][r] = min(dp[l][r], dp[l + 1][r - 1] + abs(a[l] - a[r]));
        // cout << "hhh  " << dp[l][r] << " l " << l << " r " << r << endl;
        // cout << "dp[l+1][r-1]  " << dp[l + 1][r - 1] << " abs(a[l] - a[r]) "
            //  << abs(a[l] - a[r]) << "  a[l] " << a[l] << " " << a[r] << endl;
    }

    cout << dp[0][2 * n - 1] << endl;
}