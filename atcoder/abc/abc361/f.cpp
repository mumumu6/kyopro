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
    vec p = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    vec a(10);

    rep(i, 10) { a[i] = floor( pow(n,1.0 / p[i]) ); }

    ll ans = 0;
    n      = 10;
    rep(i, n) { ans += a[i]; }
    rep(i, n - 1) reps(j, i + 1, n) { ans -= a[i] + a[j]; }

    reps(i, 0, n - 2) reps(j, i + 1, n - 1) reps(k, j + 1, n) {
        ans += a[i] + a[j] + a[k];
    }

    reps(i, 0, n - 3) reps(j, i + 1, n - 2) reps(k, j + 1, n - 1)
        reps(l, k + 1, n) {
        ans -= a[i] + a[j] + a[k] + a[l];
        cout << ans << endl;
    }

    reps(i, 0, n - 4) reps(j, i + 1, n - 3) reps(k, j + 1, n - 2)
        reps(l, k + 1, n - 1) reps(m, l + 1, n) {
        ans += a[i] + a[j] + a[k] + a[l] + a[m];
    }

    reps(i, 0, n - 5) reps(j, i + 1, n - 4) reps(k, j + 1, n - 3)
        reps(l, k + 1, n - 2) reps(m, l + 1, n - 1) reps(o, m + 1, n) {
        ans -= a[i] + a[j] + a[k] + a[l] + a[m] + a[o];
    }

    reps(i, 0, n - 6) reps(j, i + 1, n - 5) reps(k, j + 1, n - 4)
        reps(l, k + 1, n - 3) reps(m, l + 1, n - 2) reps(o, m + 1, n - 1)
            reps(p, o + 1, n) {
        ans += a[i] + a[j] + a[k] + a[l] + a[m] + a[o] + a[p];
    }

    reps(i, 0, n - 7) reps(j, i + 1, n - 6) reps(k, j + 1, n - 5)
        reps(l, k + 1, n - 4) reps(m, l + 1, n - 3) reps(o, m + 1, n - 2)
            reps(p, o + 1, n - 1) reps(q, p + 1, n) {
        ans -= a[i] + a[j] + a[k] + a[l] + a[m] + a[o] + a[p] + a[q];
    }

    reps(i, 0, n - 8) reps(j, i + 1, n - 7) reps(k, j + 1, n - 6)
        reps(l, k + 1, n - 5) reps(m, l + 1, n - 4) reps(o, m + 1, n - 3)
            reps(p, o + 1, n - 2) reps(q, p + 1, n - 1) reps(r, q + 1, n) {
        ans += a[i] + a[j] + a[k] + a[l] + a[m] + a[o] + a[p] + a[q] + a[r];
    }

    reps(i, 0, n - 9) reps(j, i + 1, n - 8) reps(k, j + 1, n - 7)
        reps(l, k + 1, n - 6) reps(m, l + 1, n - 5) reps(o, m + 1, n - 4)
            reps(p, o + 1, n - 3) reps(q, p + 1, n - 2) reps(r, q + 1, n - 1)
                reps(s, r + 1, n) {
        ans -=
            a[i] + a[j] + a[k] + a[l] + a[m] + a[o] + a[p] + a[q] + a[r] + a[s];
    }

    cout << ans << endl;
}