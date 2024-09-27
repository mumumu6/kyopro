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

    ll a, b, c;
    cin >> a >> b >> c;

    ll x = 0, y = 0;

    ll cnt = 0;

    rep(i, 60) {
        if (c & (1LL << i)) cnt++;
    }

    if (a + b < cnt) {
        cout << -1 << endl;
        return 0;
    }

    ll d = a + b - cnt;

    if ((cnt + b - a) % 2 != 0) {
        cout << -1 << endl;
        return 0;
    }

    if (cnt + a - b < 0) {
        cout << -1 << endl;
        return 0;
    } else if (cnt + b - a < 0) {
        cout << -1 << endl;
        return 0;
    }

    ll lim1 = (cnt + a - b) / 2;
    ll limy = (cnt + b - a) / 2;
    ll lim2 = a - lim1;

    rep(i, 60) {
        if (lim2 > 0 && !(c & (1LL << i))) {
            lim2--;
            y += (1LL << i);
            x += (1LL << i);
        } else if (lim1 > 0 && c & (1LL << i)) {
            lim1--;
            x += (1LL << i);
        } else if (limy > 0 && (c & (1LL << i))) {
            limy--;

            y += (1LL << i);
        }
    }
    ll i = 0;
    // while (lim2) {
    //     y += (1LL << (60 + i));
    //     x += (1LL << (60 + i));
    //     i++;

    //     lim2--;
    // }

    if (lim2 > 0) {
        cout << -1 << endl;
        return 0;
    }

    cout << x << " " << y << endl;
}