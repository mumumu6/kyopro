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
    vec l(n);
    vec r(n);
    rep(i, n) cin >> l[i] >> r[i];

    ll d   = 0;
    ll min = 0;
    rep(i, n) {
        d += r[i] - l[i];
        min += l[i];
    }

    if (min <= 0 && min + d >= 0) {
        ll dif = abs(min);
        cout << "Yes" << endl;

        rep(i, n) {
            if (dif >= r[i] - l[i]) {
                dif -= r[i] - l[i];
                cout << r[i] << " ";
            } else if (dif > 0) {
                cout << l[i] + dif << " ";
                dif = 0;
            } else {
                cout << l[i] << " ";
            }
        }

    } else {
        cout << "No" << endl;
    }
}