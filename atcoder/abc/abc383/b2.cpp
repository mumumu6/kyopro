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
vector<ll> dy = {1, 0,-1, 0};
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
    ll h, w, d;
    cin >> h >> w >> d;
    vector<vector<char>> s(h, vector<char>(w));
    rep(i, h) rep(j, w) cin >> s[i][j];

    ll ans = 0;

    rep(x1, h) rep(x2, h) rep(y1, w) rep(y2, w) {
        if (x1 == x2 && y1 == y2) continue;
        if (s[x1][y1] == '#' || s[x2][y2] == '#') continue;

        ll tmp = 0;
        rep(x, h) rep(y, w) {
            if (abs(x - x1) + abs(y - y1) > d && abs(x - x2) + abs(y - y2) > d) continue;
            if (s[x][y] == '#') continue;
            tmp++;
        }
        chmax(ans, tmp);
    }

    cout << ans << endl;
}