#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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
    cout << fixed << setprecision(20);

    ll n;
    cin >> n;

    reps(d, 1, 2000000) {
        ll ok = 1, ng = sqrt(n) + 10;
        if (n % d != 0) { continue; }
        ll val;
        ll mid;
        while (abs(ok - ng) > 1) {
            mid = (ok + ng) / 2;

            ll x = mid + d, y = mid;
            val = x * x + y * y + x * y;

            if (val <= n / d) {
                ok = mid;
            } else {
                ng = mid;
            }

            // if (d == 276544)
            //     cerr << "ok = " << ok << " ng = " << ng << " mid = " << mid 
            //           << " val = " << val << " n/d = " << n/ d  <<  "\n";
        }

        ll x = ok + d, y = ok;
        val = x * x + y * y + x * y;

        if (val == n / d) {
            cout << x << " " << y << "\n";
            return 0;
        }
    }

    cout << -1 << "\n";
}