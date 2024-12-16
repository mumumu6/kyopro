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

    string s;
    cin >> s;

    vecc memo(26, vec());
    vecc sum(26, vec());

    ll n = s.size();

    rep(i, n) { memo[s[i] - 'A'].pb(i); }

    rep(i, 26) {
        sum[i].pb(0);
        rep(j, memo[i].size()) {
            if (j == 0) {
                sum[i].pb(memo[i][j]);
                continue;
            }
            sum[i].pb(sum[i][j] + memo[i][j]);
        }
    }

    ll i = 0;

    // for (auto x : sum) {
    //     cout << i << endl;
    //     i++;
    //     for (auto y : x) cout << y << " ";
    //     cout << endl;
    // }

    ll ans = 0;

    rep(i, n) {
        char c = s[i];

        int x  = lower_bound(all(memo[c - 'A']), i) - memo[c - 'A'].begin();
        ll siz = memo[c - 'A'].size();

        ans += sum[c - 'A'][sum[c - 'A'].size() - 1] -   sum[c - 'A'][x + 1] - (siz - x - 1) * i - (siz - x - 1);
        // cout << "siz :" << siz << " x :" << x << endl;
        // cout << ans << " " << c << endl;
    }

    cout << ans << endl;
}