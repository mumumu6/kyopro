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

    ll h, w, n;
    cin >> h >> w >> n;
    string t;
    cin >> t;

    vec dx = {1, 0, -1, 0};
    vec dy = {0, 1, 0, -1};

    vector<ll> d(n);


    vector<string> s(h);
    rep(i, h) cin >> s[i];

    rep(i, n) {
        if (t[i] == 'U') d[i] = 2;
        if (t[i] == 'D') d[i] = 0;
        if (t[i] == 'L') d[i] = 3;
        if (t[i] == 'R') d[i] = 1;
    }
    ll ans = 0;

    auto f = [&](auto f, pll p, ll x) {
        if (p.ft + dx[d[x]] < 0 || p.ft + dx[d[x]] >= h || p.sd + dy[d[x]] < 0 || p.sd + dy[d[x]] >= w)
            return;

        if (s[p.ft + dx[d[x]]][p.sd + dy[d[x]]] == '.') {
            if (x == n - 1) {
                ans++;
                return;
            }
            f(f, mp(p.ft + dx[d[x]], p.sd + dy[d[x]]), x + 1);
        }
    };

    rep(i, h) rep(j, w) {
        if (s[i][j] == '.') {
            f(f, mp(i, j), 0);
        }
    }

    cout << ans << endl;
}