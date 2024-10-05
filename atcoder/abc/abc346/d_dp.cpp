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
    string s;
    cin >> n >> s;

    vec c(n);
    rep(i, n) cin >> c[i];

    vector dp(n + 1, vecc(2, vec(2, 4e18)));


    dp[0][0][s[0] - '0'] = 0, dp[0][0][(s[0] - '0') ^ 1] = c[0];

    rep(i, n - 1) {
        dp[i + 1][0][0] = dp[i][0][1] + (s[i + 1] == '1' ? c[i + 1] : 0);

        dp[i + 1][0][1] = dp[i][0][0] + (s[i + 1] == '0' ? c[i + 1] : 0);

        dp[i + 1][1][0] = min(dp[i][1][1] + (s[i + 1] == '1' ? c[i + 1] : 0), dp[i][0][0] + (s[i + 1] == '1' ? c[i + 1] : 0));

        dp[i + 1][1][1] = min(dp[i][1][0] + (s[i + 1] == '0' ? c[i + 1] : 0), dp[i][0][1] + (s[i + 1] == '0' ? c[i + 1] : 0));
    }



    cout << min({dp[n - 1][1][0], dp[n - 1][1][1]}) << endl;
}