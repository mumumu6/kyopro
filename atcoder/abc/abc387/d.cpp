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
vector<ll> dx = {0, 0, 1, -1};
vector<ll> dy = {1, -1, 0, 0};
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

    ll h, w;

    cin >> h >> w;
    vector<vector<char>> s(h, vector<char>(w));
    pll st, go;
    rep(i, h) rep(j, w) {
        cin >> s[i][j];

        if (s[i][j] == 'S') st = mp(i, j);
        if (s[i][j] == 'G') go = mp(i, j);
    }

    queue<pair<pll, pll>> q;

    vector<vector<vector<bool>>> used(h, vector<vector<bool>>(w, vector<bool>(2, false)));

    rep(i, 4) { //
        ll x = st.ft;
        ll y = st.sd;

        ll nx = x + dx[i];
        ll ny = y + dy[i];

        if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
        if (s[nx][ny] == '#') continue;

        q.push(mp(mp(nx, ny), mp(i / 2, 1)));
        used[nx][ny][i / 2] = true;
    }

    while (!q.empty()) {
        auto [p, d] = q.front();
        q.pop();
        auto [x, y]     = p;
        auto [dir, cnt] = d;
        if (s[x][y] == 'G') {
            cout << cnt << endl;
            return 0;
        }

        rep(i, 4) {
            ll nx = x + dx[i];
            ll ny = y + dy[i];

            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (s[nx][ny] == '#') continue;

            if (dir == i / 2) {
                continue;
            } else {
                if (used[nx][ny][i / 2]) continue;
                q.push(mp(mp(nx, ny), mp(i / 2, cnt + 1)));
                used[nx][ny][i / 2] = true;
            }
        }
    }

    cout << -1 << endl;
}
