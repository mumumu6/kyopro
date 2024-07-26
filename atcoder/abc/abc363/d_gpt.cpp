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

    ll n;
    cin >> n;

    ll a = n;
    if (a <= 10) {
        cout << a - 1 << endl;
        return 0;
    }

    int d = 1;
    a -= 1;
    int cnt = 0;

    while (a > 0) {
        ll max_for_d = 9 * pow(10, cnt);
        if (a <= max_for_d) break;
        a -= max_for_d;
        cnt++;
    }
    d = pow(10, cnt);
    a += 18 * (d / 10);

    if (a <= 9 * d) {
        ll k = pow(10, cnt);
        ll b = a + k - 1;
        string c = to_string(b);
        string cc;

        rep(i, c.size() - 1) cc.push_back(c[c.size() - 2 - i]);
        cout << c << cc << endl;
    } else {
        ll aa = a - 9 * d;
        ll k  = pow(10, cnt);
        string ccc = to_string(aa + k - 1);
        cout << ccc;
        reverse(all(ccc));
        cout << ccc << endl;
    }

    return 0;
}
