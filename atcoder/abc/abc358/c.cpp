#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<bool>> s(n, vector<bool>(m));
    rep(i, n) rep(j, m) {
        char c;
        cin >> c;
        s[i][j] = (c == 'o') ? true : false;
    }
    int ans = 1010101010;
    for (int bit = 0; bit < (1 << n); bit++) {
        int cnt    = 0;
        bool check = true;
        vector<bool> c(m, false);
        rep(i, n) {
            if (bit & (1 << i)) {
                cnt++;
                rep(j, m) c[j] = c[j] || s[i][j];
            }
        }
        rep(i, m) {
            if (!c[i]) check = false;
        }

        if (check) {
            ans = min(ans, cnt);
        }
    }
    cout << ans << endl;
}