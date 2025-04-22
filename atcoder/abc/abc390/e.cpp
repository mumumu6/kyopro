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

    vector<ll> v(n);
    vector<ll> a(n);
    vector<ll> c(n);

    rep(i, n) {
        cin >> v[i] >> a[i] >> c[i];
        v[i]--;
    }

    vector<vector<vector<ll>>> dp(3, vector<vector<ll>>(n + 1, vector<ll>(x + 1, 0)));

    reps(i, 0, n) rep(w, x) {
        rep(j, 3) dp[j][i + 1][w] = dp[j][i][w];

        if (w >= c[i]) chmax(dp[v[i]][i + 1][w], dp[v[i]][i][w - c[i]] + a[i]);

        // cout << "dp[" << v[i] << "][" << i + 1 << "][" << w << "] : " << dp[v[i]][i + 1][w] << endl;
    }

    auto f = [&](ll w, ll k) -> ll {
        // w以上のビタミンをとるのに必要な最低カロリーを返す

        ll ok = x;
        ll ng = -1;

        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;

            if (dp[k][n][mid] >= w) {
                ok = mid;
                // デバッグ出力: f関数内
                // cout << "ok dp[" << k << "][" << n << "][" << mid
                //      << "] : " << dp[k][n][mid] << endl;
            } else {
                ng = mid;
                // デバッグ出力: f関数内
                // cout << "ng dp[" << k << "][" << n<< "][" << mid
                //      << "] : " << dp[k][n][mid] << endl;
            }
        }

        return ok;
    };

    ll ok = 0;
    ll ng = 4e8;

    while (abs(ok - ng) > 1) {
        ll mid = (ok + ng) / 2;

        // mid以上のビタミンをとるのにそれぞれの必要な最低カロリーがしりたい
        ll f0 = f(mid, 0);
        ll f1 = f(mid, 1);
        ll f2 = f(mid, 2);

        if (f0 + f1 + f2 <= x) {
            // デバッグ出力: メインループ内
            // cout << "ok f(" << mid << ", 0) : " << f0 << ", f(" << mid << ", 1) : " << f1
            //      << ", f(" << mid << ", 2) : " << f2 << endl;
            ok = mid;
        } else {
            ng = mid;
            // cout << "ng f(" << mid << ", 0) : " << f0 << ", f(" << mid << ", 1) : " << f1
            // << ", f(" << mid << ", 2) : " << f2 << endl;
        }
    }

    cout << ok << endl;
}
