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

    ll n, m;
    cin >> n >> m;

    vec a(n);
    rep(i, n) cin >> a[i];

    vec sum(n + 1, 0);

    sort(all(a));

    rep(i, n) { sum[i + 1] = sum[i] + a[i]; }

    ll ok  = 0;
    ll ng = 4e18;

    if (sum[n] <= m) {
        cout << "infinite" << endl;
        return 0;
    }

    auto f = [&](ll x) -> bool {
        auto itr = lower_bound(all(a), x) - a.begin();

        if (sum[itr] + x * (n - itr) > m)
            return false;
        else
            return true;
    };

    while (abs(ok - ng) > 1) {
        ll mid = (ok + ng) / 2;

        if (f(mid))
            ok = mid;
        else
            ng = mid;
    }

    cout << ok << endl;
}