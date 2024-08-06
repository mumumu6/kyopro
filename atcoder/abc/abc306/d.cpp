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

    ll n;
    cin >> n;
    vec a(n), b(n);
    rep(i, n) cin >> a[i] >> b[i];

    vecc dp(n+1,vec(2,-1e9));
    dp[0][0] = 0;

    reps(i,1,n+1){
        if(a[i-1]== 0){
            dp[i][0] = max({dp[i-1][0]+ b[i-1],dp[i-1][1]+ b[i-1], dp[i-1][0]});
            dp[i][1] = dp[i-1][1];
        }
        if(a[i-1] == 1){
            dp[i][0] = dp[i-1][0];
            dp[i][1] = max({ dp[i-1][0] + b[i-1],dp[i-1][1]});
        }
    }    

    cout << max(dp[n][0],dp[n][1]) << endl;
}