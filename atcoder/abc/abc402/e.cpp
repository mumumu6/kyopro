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
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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

    ll n, x;
    cin >> n >> x;

    vector<ll> s(n);
    vector<ll> c(n);
    vector<ll> p(n);

    rep(i, n) { cin >> s[i] >> c[i] >> p[i]; } // 期待値を計算

    vector dp(1 << n, vector<double>(x + 1, 0.0));

    rep(i, 1 << n) rep(now, x + 1) {
        rep(j, n) {
            if (!(i & (1 << j))) {
                if (x - c[j] >= 0) {
                    chmax(dp[i | (1 << j)][now - c[j]], (dp[i][now] + s[j]) * double(p[j]) / 100.0 +
                                                            dp[i][now] * (1.0 - double(p[j]) / 100.0));
                }
            }
        }
    }
    double ans = 0;
    rep(i, 1 << n) {
        rep(j, x + 1) { chmax(ans, dp[i][j]); }
    }
    cout << ans << endl;
    return 0;
}
