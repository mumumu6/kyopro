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
    cout << fixed << setprecision(10);

    ll t, l, x, y, q;
    cin >> t >> l >> x >> y >> q;

    rep(i, q) {
        ll e;
        cin >> e;
        double c  = -e * 2 * 3.141592653589 / t - 3.141592653589 / 2;
        double dz = (l / 2.0) * (sin(c) + 1.0);

        // cout << " dy " << dy << " x " << x << endl;
        cout << atan2(dz, sqrt(x * x + (y - (l / 2.0) * cos(c)) *
                                           (y - (l / 2.0) * cos(c)))) *
                    180 / 3.1415926535
             << endl;
    }
}