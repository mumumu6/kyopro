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
    int n, q;
    cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    cin >> q;
    vector<int> b(q);
    rep(i, q) cin >> b[i];

    sort(a.begin(), a.end());

    rep(i, q) {
        auto la = lower_bound(a.begin(), a.end(), b[i]) - a.begin();
        int ans = abs(a[la] - b[i]);
        if (la != 0)  ans = min(ans, abs(a[la - 1] - b[i]));

        cout << ans << endl;
    }
}