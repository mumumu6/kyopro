#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint1000000007;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(m);
    rep(i, m) cin >> a[i];
    sort(a.begin(), a.end());

    vector<mint> dp(n + 1, 0);
    dp[0] = 1;

    set<int> skip(a.begin(), a.end());

    rep(i,n+1) {
        if (skip.count(i) == 0) {
            if (i + 1 <= n && skip.count(i + 1) == 0) {
                dp[i + 1] += dp[i];
            }
            if (i + 2 <= n && skip.count(i + 2) == 0) {
                dp[i + 2] += dp[i];
            }
        }
    }       

    cout << dp[n].val() << endl;
}
