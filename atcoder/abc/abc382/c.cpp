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

    ll n, m;
    cin >> n >> m;
    vec a(n);
    vec b(m);
    vector<pll> c;
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    c.push_back(mp(a[0], 0));

    rep(i, n) {
        if (c.back().ft > a[i]) { c.push_back(mp(a[i], i)); }
    }
    sort(all(c));

 

    rep(i, m) {
        if (b[i] < c[0].ft) {
            cout << -1 << endl;
            continue;
        }
        auto it = upper_bound(all(c), mp(b[i], 0LL), [](auto x, auto y ){
            return x.ft < y.ft;
        })  - c.begin();
        it--;
        
        
        cout << c[it].sd + 1 << endl;
    }
}
// 60 45 37 22
// 22 37 45 60
// 70 39 52 33 18