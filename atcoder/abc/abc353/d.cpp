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
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n;
    cin >> n;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];

    mint ans = 0;

    rep(i, n) ans += mint(i) * mint(a[i]);
    vector<vector<ll>> s(n + 1, vector<ll>(11, 0));

    rep(i, n) {
        s[i + 1] = s[i];
        s[i + 1][to_string(a[i]).size()]++;
    }
    // cout << ans.val() << endl;
    reps(i, 1, n + 1) {
        mint d = 0;

        rep(j, 11) {
            d += mint(10).pow(j) * (s[n][j] - s[i][j]);
            // cout << s[n][j] - s[i][j] << " " << j <<endl;;
        }
        // cout << d.val() << endl;
        ans += d * mint(a[i - 1]);
        mint c = d * mint(a[i - 1]);
        // cout << c.val() << endl;
        //  cout  << "ans"<< ans.val()<< endl;
    }
    cout << ans.val() << endl;
}