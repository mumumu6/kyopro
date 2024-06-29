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
    ll n;
    cin >> n;
    vector<ll> a(n);
    ll sum   = 0;
    ll count = 0;

    rep(i, n) {
        ll x, y;
        cin >> x >> y;
        sum += x;
        a[i] = ll(2) * x + y;
    }

    sort(a.begin(), a.end(), greater<ll>());

    while (sum >= 0) {
        sum -= a[count];
        count++;
    }
    cout << count << endl;
}