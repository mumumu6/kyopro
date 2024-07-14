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

vector<int> dx = {1, 0, -1, 0};
vector<int> dy = {0, 1, 0, -1};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int h, w;
    cin >> h >> w;
    vector<string> s(h);
    rep(i, h) cin >> s[i];

    vector<vector<bool>> x(h, vector<bool>(w, false));
    rep(i, h) rep(j, w) {
        if (s[i][j] != '#') continue;
        x[i][j] = true;
        rep(v, 4) {
            int nx = i + dx[v];
            int ny = j + dy[v];
            if (0 <= nx && nx < h && 0 <= ny && ny < w) x[nx][ny] = true;
        }
    }
    vector<vector<bool>> used(h, vector<bool>(w, false));
    vector last(h, vector<int>(w, 0));
    int t   = 0;
    int ans = 1;
    rep(si, h) rep(sj, w) {
        if (used[si][sj]) continue;
        if (x[si][sj]) continue;
        t++;

        // cout << "たんさく！！   x " << si << " y " << sj << endl;

        queue<pii> q;
        int cnt = 1;
        q.push({si, sj});
        used[si][sj] = true;

        while (!q.empty()) {
            auto [i, j] = q.front();
            q.pop();

            rep(v, 4) {
                int nx = i + dx[v];
                int ny = j + dy[v];

                if (nx < 0 || nx >= h || ny < 0 || ny >= w || used[nx][ny])
                    continue;

                if (x[nx][ny]) {
                    if (last[nx][ny] != t) {
                        cnt++;
                        last[nx][ny] = t;
                        // cout << nx << " " << ny << "  tは" << t << " xは" << i
                            //  << "yは" << j << endl;
                    }
                    continue;
                }

                q.push({nx, ny});
                cnt++;
                // cout << "new!!!   nx " << nx << " ny " << ny << endl;
                used[nx][ny] = true;
            }
        }
        ans = max(ans, cnt);
    }

    cout << ans << endl;
}