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
    vec a(n);
    vec cnt(10, 0);
    rep(i, n) cin >> a[i];
    ll ans = 0;

    ll l = 0, r = 0;

    while (r < n) {
        if (r + 1 < n && a[r] == a[r + 1] && cnt[a[r]] == 0) {
            cnt[a[r]]++;
            r += 2;
            chmax(ans, r - l);

        } else {
            ll tl = l;
            ll tr = r;
            l     = tr + 2;
            r     = tr + 2;
            cnt   = vec(10, 0);
        }
        // cout << "l: " << l << " r: " << r << endl;
        // cout << "a[l]: " << a[l] << " a[r]: " << a[r] << endl;
    }

    l = 1, r = 1;
    cnt = vec(10, 0);
    while (r < n) {
        if (r + 1 < n && a[r] == a[r + 1] && cnt[a[r]] == 0) {
            cnt[a[r]]++;
            r += 2;
            chmax(ans, r - l);

        } else {
            ll tl = l;
            ll tr = r;
            l     = tr + 2;
            r     = tr + 2;
            cnt   = vec(10, 0);
        }
        // cout << "l: " << l << " r: " << r << endl;
        // cout << "a[l]: " << a[l] << " a[r]: " << a[r] << endl;
    }
    cout << ans << endl;
}