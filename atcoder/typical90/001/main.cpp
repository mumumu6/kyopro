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
    int prev = 0;
    rep(i, n) {
        int x;
        cin >> x;
        a.push_back(x - prev);
        prev = x;
    }
    a.push_back(l - prev);

    // ok以上の長さなら洋館を切る

    int ok = -1;
    int ng = 1e9 + 1;
    while (abs(ok - ng) > 1) {
        int mid = (ok + ng) / 2;

        bool good = [&]() {
            int pieces = 0;
            int now    = 0;

            for (auto b : a) {
                now += b;
                if (now >= mid) {
                    pieces++;
                    now = 0;
                }
            }

            return pieces >= k + 1;
        }();

        if (good)
            ok = mid;
        else
            ng = mid;
    }

    cout << ok << endl;
}