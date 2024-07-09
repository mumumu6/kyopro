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
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int r, c;
    int sy, sx, gy, gx;
    cin >> r >> c >> sy >> sx >> gy >> gx;
    sx--;
    sy--;
    gx--;
    gy--;

    vector<vector<char>> s(r, vector<char>(c));
    rep(i, r) rep(j, c) cin >> s[i][j];

    const vector<int> dx = {1, 0, -1, 0};
    const vector<int> dy = {0, 1, 0, -1};

    queue<pair<int, int>> q;
    vector<vector<int>> dp(r, vector<int>(c, -1));

    q.push({sy, sx});
    dp[sy][sx] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        rep(i, 4) {
            int x2 = x + dx[i];
            int y2 = y + dy[i];

            if (x2 < 0 || x2 >= r || y2 < 0 || y2 >= c) continue;
            if (s[x2][y2] == '#') continue;
            if (dp[x2][y2] != -1) continue;
            q.push({x2, y2});
            dp[x2][y2] = dp[x][y] + 1;

            // cout << dp[x2][y2] << ' ' << x2 << ' ' << y2 << endl;
        }
    }

    cout << dp[gy][gx] << '\n';
}