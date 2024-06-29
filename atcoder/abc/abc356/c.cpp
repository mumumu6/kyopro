#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int>> a(m);
    vector<char> r(m);

    rep(i, m) {
        int c;
        cin >> c;
        a[i].resize(c);
        rep(j, c) { cin >> a[i][j]; }
        cin >> r[i];
    }
    int ans = 0;
    for (int bit = 0; bit < (1 << n); bit++) {
        set<int> s;
        rep(i, n) {
            if ((bit >> i) & 1) {
                s.insert(i+1);
            }
        }

        bool check = true;

        rep(i, m) {
            int cnt = 0;
            for (int t : a[i]) {
                if (s.find(t) != s.end()) {
                    cnt++;
                }
            }
            if (cnt < k && r[i] == 'o') check = false;
            if (cnt >= k && r[i] == 'x') check = false;
        }
        if (check) ans++;
    }
    cout << ans << endl;
}
