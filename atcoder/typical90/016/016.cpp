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

    ll n, a, b, c;
    cin >> n >> a >> b >> c;

    int m  = 9999;
    ll ans = 4e8;

    rep(x, m) rep(y, m - x) {
        if ((n - a * x - b * y) % c != 0 || n - a * x - b * y < 0) continue;
        int z = (n - a * x - b * y) / c;

        ans = min(ans, x + y + z);
    }

    cout << ans << endl;
}