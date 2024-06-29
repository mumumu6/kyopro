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

ll inf = 4e18;

int main() {
    ll n, sx, sy, gx, gy;
    cin >> n >> sx >> sy >> gx >> gy;

    vector<vector<bool>> sensor(1000, vector<bool>(1000, false));
    ll fasensor = 0;

    rep(i, n) {
        ll x, y, r;
        cin >> x >> y >> r;
        if (abs(sx - x) <= r && abs(sy - y) <= r) {
            fasensor++;
            continue;
        }

        reps(j, -r, r + 1) { // jは-rからrまで
            if (x - r < 0 || y + j >= 1000 || y + j < 0) continue;
            sensor[x - r][y + j] = true;
        }

        reps(j, -r, r + 1) { // jは-rからrまで
            if (y + r >= 1000 || x + j < 0 || x + j >= 1000) continue;
            sensor[x + j][y + r] = true;
        }
    }

    vector<vector<int>> dp(1000, vector<int>(1000, inf));
    ll dx = abs(gx - sx);
    ll dy = abs(gy - sy);

    dp[sx][sy] = fasensor;

    rep(i, dx) rep(j, dy) {

        if (sensor[sx + i][sy - j]) dp[sx + i ][sy]++;
        dp[sx + i + 1][sy - j] =
            min(dp[sx + i][sy - j], dp[sx + i + 1][sy + j]);

        if (sensor[sx + i][sy - j]) dp[sx + i ][sy]--;

        if (sensor[sx + i][sy - j - 1]) dp[sx+i][sy - j ]++;

        dp[sx + i][sy - j - 1] =
            min(dp[sx + i][sy - j], dp[sx + i][sy - j - 1]);

         if (sensor[sx + i][sy - j - 1]) dp[sx+i][sy - j ]--;
        
    }

    cout << dp[gx][gy] << endl;
}