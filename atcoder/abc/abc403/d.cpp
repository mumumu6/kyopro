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

    ll n, d;
    cin >> n >> d;

    vec a(n);

    rep(i, n) cin >> a[i];

    sort(all(a));

    if (d == 0) {
        set<ll> st;
        rep(i, n) st.insert(a[i]);
        cout << n - st.size() << endl;
        return 0;
    }

    ll m = a[n - 1];

    vector<ll> c(m + 1, 0);

    rep(i, n) { c[a[i]]++; }

    ll ans = 0;

    rep(i, d) {
        vector<ll> dp((m - i) / d + 1, 0);

        if (i + d <= m) {
            dp[1] = min(c[i + d], c[i]); // 使わない、使う
        }

        reps(j, 1, (m - i) / d) {
            dp[j + 1] = min(dp[j] + c[i + (j + 1) * d], dp[j - 1] + c[i + j * d]); // 使わない、使う
            // cout << j << " " << dp[j] << " " << dp[j + 1] << e   ndl;
        }

        ans += dp[(m - i) / d];
        // cout << i << " " << dp[(m - i) / d] << " " << (m - i) / d  << endl;
        // cout << "----------" << endl;
    }

    cout << ans << endl;
}