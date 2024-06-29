#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n, t;
    cin >> n >> t;
    vector<int> a(n, 0); // 行
    vector<int> b(n, 0); // 列
    int aa  = 0;
    int bb  = 0;
    int ans = -1;

    rep(i, t) {
        int c;
        cin >> c;
        a[(c - 1) / n]++;
        b[(c - 1) % n]++;
        if ((c - 1) / n == (c - 1) % n) aa++;
        if ((c - 1) / n + (c - 1) % n == n-1)bb++;
            if (a[(c - 1) / n] == n || b[(c - 1) % n] == n || aa == n || bb == n) {
                ans = i + 1;
                break;
            }
    }
    cout << ans << endl;
}