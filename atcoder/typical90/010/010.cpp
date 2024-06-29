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

int main() {
    int n;
    cin >> n;

    vector<int> sum1(n + 1);
    vector<int> sum2(n + 1);
    sum1[0] = 0;
    sum2[0] = 0;

    rep(i, n) {
        int c, p;
        cin >> c >> p;

        if (c == 1) {
            sum1[i + 1] = sum1[i] + p;
            sum2[i + 1] = sum2[i];
        } else {
            sum1[i + 1] = sum1[i];
            sum2[i + 1] = sum2[i] + p;
        }
    }

    int q;
    cin >> q;

    rep(i, q) {
        int l, r;
        cin >> l >> r;

        cout << sum1[r] - sum1[l-1] << " " << sum2[r] - sum2[l-1] << endl;
    }
}