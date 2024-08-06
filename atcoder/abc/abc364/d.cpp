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
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    sort(all(a));

    while (q--) {
        int b, k;
        cin >> b >> k;
        auto f = [&](int x) {
            auto lb = lower_bound(a.begin(), a.end(), b - x);
            auto ub = upper_bound(a.begin(), a.end(), b + x);
            return ub - lb >= k;
        };
        int ng = -1, ok = 2e8 + 10;
        while (abs(ok - ng) > 1) {
            int mid = (ok + ng) / 2;
            if (f(mid)) {
                ok = mid;
            } else {
                ng = mid;
            }
        }
        cout << ok << endl;
    }

}