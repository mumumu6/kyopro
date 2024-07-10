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
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll a, b, c, d;
    cin >> a >> b >> c >> d;
    bool bch = false;
    bool dch = false;
    if ((b % 2+2) % 2 == 1) {
        b--;
        bch = true;
    }
    if ((d % 2 + 2) % 2 == 1) {
        d++;
        dch = true;
    }
    ll dy = d - b;
    ll dx = c - a;

    ll ans = 0;

    vector<ll> aa = {3, 3, 1, 1, 3, 3, 1, 1};
    vector<ll> dd = {1, 2, 1, 0, 1, 2, 1, 0};
    vector<ll> bb = {2, 1, 0, 1, 2, 1, 0, 1};

    ll Sx   = 0;
    ll Sddx = 0;
    ll j    = (a % 4 + 4) % 4;
    // cout << j << endl;
    // cout << j << endl;
    // cout << Sx << endl;
    rep(i, 4) {
        Sx += aa[j];
        if (bch) Sddx += bb[j];
        if (dch) Sddx += dd[j];

        j++;
        // cout << Sx << endl;
    }

    ans += Sx * (dy / 2) * (dx / 4);
    int ddx  = 0;
    int dddx = 0;
    j        = (a % 4 + 4) % 4;
    rep(i, dx % 4) {

        ddx += aa[j];
        if (bch) dddx += bb[j];
        if (dch) dddx += dd[j];
        j++;
        // cout << dddx << endl;
    }

    ans += ddx * (dy / 2);

    ans -= (Sddx * (dx / 4) + dddx);

    cout << ans << endl;
}