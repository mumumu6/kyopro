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
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(10);

    ll n, m;
    mint ans = 0;
    cin >> n >> m;
    

    for (ll i = 0, p2 = 1; m > 0; i++, p2 *= 2) {

        int d = m % 2;
        m /= 2;

        if (d == 0) continue;

        if (n % (p2 * 2) > p2) {
            ans += (n / (p2 * 2)) * p2 + n % (p2 * 2) - p2;
        } else {
            ans +=( n / (p2 * 2)) * p2;
        }
    }
    cout << ans.val() << endl;
}