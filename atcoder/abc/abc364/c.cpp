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

    ll n, x, y;
    cin >> n >> x >> y;
    vector<pair<ll, ll>> a(n);
    vector<pair<ll, ll>> b(n);
    rep(i, n) cin >> a[i].ft;
    rep(i, n) cin >> a[i].sd;

    rep(i, n) {
        b[i].ft = a[i].sd;
        b[i].sd = a[i].ft;
    }

    sort(a.begin(), a.end(), greater<pair<ll, ll>>());
    sort(b.begin(), b.end(), greater<pair<ll, ll>>());
    ll sum = 0;
    ll d = 0, e = 0;
    while (sum <= x && d < n) {
        sum += a[d].ft;
        d++;
        // cout << "d " << d << " sum " << sum << endl;
    }

    sum = 0;
    while (sum <= y && e < n) {
        sum += b[e].ft;
        e++;
        // cout << "e " << e << " sum " << sum << endl;
    }
    cout << min(d, e) << endl;
}