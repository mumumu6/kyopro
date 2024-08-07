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

    vec a(4e5, 0);

    ll q;
    cin >> q;

    ll l = 1e5 + 10, r = 1e5 + 10;

    rep(i, q) {
        int t;
        cin >> t;
        if (t == 1) {
            int x;
            cin >> x;
            a[r] = x;
            r++;
        } else if (t == 2) {
            int x;
            cin >> x;
            a[l - 1] = x;
            l--;
        } else {
            int x;
            cin >> x;
            cout << a[r - x] << endl;
        }
    }
}