#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'
#define debug(x) cerr << #x << " = " << x << el

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll t;
    cin >> t;

    rep(ti, t) {
        ll n;
        cin >> n;

        vec a(2 * n);
        rep(i, 2 * n) {
            cin >> a[i];
            a[i]--;
        }

        vecc c(n + 1);

        ll ans = 0;

        set<pll> s;

        rep(i, 2 * n) { c[a[i] + 10].pb(i); }

        rep(i, 2*n - 1) {
            ll t1 = a[i];
            ll t2 = a[i + 1];

            if(t1 == t2)continue;

            ll x1 = c[t1][0];
            ll x2 = c[t1][1];
            ll y1 = c[t2][0];
            ll y2 = c[t2][1];

            
            if(abs(x1 - x2) == 1 || abs(y1 - y2) == 1 ) continue;
            // cerr << "x1: " << x1 << " x2: " << x2 << " y1: " << y1 << " y2: " << y2 << el;

            if (abs(x1 - y1) == 1 && abs(x2 - y2) == 1 ) { s.insert({min(t1, t2), max(t1, t2)}); }
        }

        cout << s.size() << el;
    }
}