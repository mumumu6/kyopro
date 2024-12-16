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

    ll n;
    cin >> n;
    vec a(n);
    vec b(n - 1);

    rep(i, n) cin >> a[i];
    rep(i, n - 1) cin >> b[i];

    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());

    // for (auto x : a) cout << x << " ";
    // cout << endl;
    // for (auto x : b) cout << x << " ";
    // cout << endl;
    ll ans = 0;
    ll j   = 0;

    rep(i, n) {

        if (a[i] > b[j] && ans != 0) {
            cout << -1 << endl;
            return 0;
        } else if (a[i] > b[j]) {
            ans = a[i];
            continue;
        }

        j++;
    }

    cout << ans << endl;
    return 0;
}