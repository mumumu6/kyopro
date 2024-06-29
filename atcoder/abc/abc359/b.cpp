#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n;
    cin >> n;
    int ans = 0;
    vector<int> a(2 * n);
    rep(i, 2 * n) cin >> a[i];

    rep(i, 2*n - 2) {
        if (a[i] == a[i + 2]) {
            ans++;
           
        }
    }

    cout << ans << endl;
}