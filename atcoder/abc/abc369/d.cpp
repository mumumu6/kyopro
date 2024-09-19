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
    vec a(n);
    rep(i, n) cin >> a[i];

    vector<vector<ll>> dp(n + 1, vector<ll>(4, 0));
    dp[0][0] = 0;
    dp[0][1] = -4e18;
    dp[0][2] = -4e18;
    dp[0][3] = a[0];
    reps(i, 1, n) {
        // 食べない
        dp[i][0] = max(dp[i - 1][0], dp[i - 1][2]);
        dp[i][1] = max(dp[i - 1][1], dp[i - 1][3]);
        // 食べる
        dp[i][2] = 2 * a[i] + max(dp[i - 1][1], dp[i - 1][3]);
        dp[i][3] = a[i] + max(dp[i - 1][0], dp[i - 1][2]);
    }

    cout << max({dp[n-1][0], dp[n-1][1], dp[n-1][2], dp[n-1][3]}) << endl;
}