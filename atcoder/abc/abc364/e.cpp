
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (n); i++)
#define reps(i, a, b) for (int i = (a); i < (b); i++)
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

    int n, x, y;
    cin >> n >> x >> y;
    vector<pair<int, int>> dis(n);
    rep(i, n) cin >> dis[i].ft >> dis[i].sd;

    vector<vector<vector<int>>> dp(
        n + 1, vector<vector<int>>(x + 1, vector<int>(y + 1, 0)));

    reps(i, 1, n + 1) {
        int a = dis[i - 1].ft;
        int b = dis[i - 1].sd;

        rep(j, x + 1) rep(k, y + 1) {
            dp[i][j][k] = dp[i - 1][j][k];
            if (j >= a && k >= b)
                dp[i][j][k] = max(dp[i][j][k], dp[i - 1][j - a][k] + 1);
        }
    }
    if (dp[n][x][y] == n) {
        cout << n << endl;
        return 0;
    }
    cout << dp[n][x][y] + 1 << endl;
}