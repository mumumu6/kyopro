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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll h, w, x, y;
    cin >> h >> w >> x >> y;
    x--, y--;
    vector<vector<char>> s(h, vector<char>(w));
    rep(i, h) rep(j, w) cin >> s[i][j];
    string t;
    cin >> t;

    ll n = t.size();

    ll ans = 0;
    vector<vector<bool>> used(h, vector<bool>(w, false));
    if (s[x][y] == '@') {
        ans++;
        used[x][y] = true;
    }

    rep(i, n) {
        if (t[i] == 'U') {
            if (x > 0 && s[x - 1][y] != '#') x--;

            if (s[x][y] == '@' && !used[x][y]) {
                ans++;
                used[x][y] = true;
            }
        } else if (t[i] == 'D') {
            if (x < h - 1 && s[x + 1][y] != '#') x++;

            if (s[x][y] == '@' && !used[x][y]) {
                ans++;
                used[x][y] = true;
            }
        } else if (t[i] == 'L') {
            if (y > 0 && s[x][y - 1] != '#') y--;

            if (s[x][y] == '@' && !used[x][y]) {
                ans++;
                used[x][y] = true;
            }
        } else if (t[i] == 'R') {
            if (y < w - 1 && s[x][y + 1] != '#') y++;

            if (s[x][y] == '@' && !used[x][y]) {
                ans++;
                used[x][y] = true;
            }
        }
    }

    cout << x + 1 << " " << y + 1 << " " << ans << endl;
}