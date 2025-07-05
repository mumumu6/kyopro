#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int k, s;
    cin >> k >> s;
    int ans = 0;
    rep(i, k+1) rep(j, k+1) {
        if (i + j <= s && s- i - j <= k) ans++;
    }
    cout << ans << endl;
}