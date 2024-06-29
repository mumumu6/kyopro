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
int main() {
    ll n, q;
    cin >> n >> q;
    vector<ll> r(n);
    rep(i, n) cin >> r[i];

    sort(r.begin(), r.end());

    vector<ll> rps(n + 1,0);
    rep(i, n) rps[i + 1] = rps[i] + r[i];

    rep(i, q) {
        ll x;
        cin >> x; 

        auto it = upper_bound(rps.begin(), rps.end(), x) - rps.begin() -1;
        cout << it << endl;
    }
}