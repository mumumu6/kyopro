#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n, l, k;
    cin >> n >> l >> k;
    vector<int> a;
    int pre = 0;
    rep(i, n) {
        int b;
        cin >> b;
        a.push_back(b - pre);
        pre = b;
    }
    a.push_back(l - pre);

    int ok = -1;
    int ng = 1e9 + 1;

    while (abs(ok - ng) > 1) {
        int mid = (ok + ng) / 2;

        bool isOk = [&]() {
            int now = 0;
            int cnt = 0;

            rep(i, n+1) {
                now += a[i];
                if (now >= mid) {
                    cnt++;
                    now = 0;
                }
            }

            return (cnt > k);
        }();

        if (isOk) ok = mid;
        else ng = mid;
    }
    cout << ok << endl;
}