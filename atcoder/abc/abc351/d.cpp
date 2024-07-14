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

    int h, w;
    cin >> h >> w;
    vector<string> s(h);
    rep(i, h) cin >> s[i];

    ll ans = 0;

    vector<int> dx = {1, 0, -1, 0};
    vector<int> dy = {0, 1, 0, -1};

    vector<vector<int>> used(h, vector<int>(w, 0));
    rep(i, h) rep(j, w) {
        if (s[i][j] == '#') continue;
        if (used[i][j]) continue;
        queue<pair<int, int>> que;
        ll cnt = 0;
        vector<vector<bool>> check((h), vector<bool>(w, false));
        que.push({i, j});
        check[i][j] = true;
        used[i][j]  = true;

        while (!que.empty()) {
            auto [x, y] = que.front();
            check[x][y] = true;
            que.pop();
            cnt++;
            bool stop = false;
            rep(k, 4) {
                int nx = x + dx[k];
                int ny = y + dy[k];

                if (0 <= nx && nx < h && 0 <= ny && ny < w &&
                    s[nx][ny] == '#') {
                    stop = true;
                    break;
                }
            }

            if (stop) continue;
            used[x][y] = true;

            rep(k, 4) {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (0 > nx || nx >= h || 0 > ny || ny >= w) continue;

                if (!check[nx][ny]) {
                    que.push({nx, ny});
                    check[nx][ny] = true;
                }
            }
        }
        ans = max(ans, cnt);
    }
    cout << ans << endl;
}