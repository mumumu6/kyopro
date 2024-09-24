#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (n); i++)
#define reps(i, a, b) for (int i = (a); i < (b); i++)
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

    int h, w;
    cin >> h >> w;

    vec dx = {1, 0, -1, 0};
    vec dy = {0, 1, 0, -1};
    int sx, sy;
    vector<vector<char>> a(h, vector<char>(w));
    rep(i, h) rep(j, w) {
        cin >> a[i][j];
        if (a[i][j] == 'S') {
            sx = i;
            sy = j;
        }
    }

    int n;
    cin >> n;
    vecc e(h, vec(w, 0));
    rep(i, n) {
        int r, c, ee;
        cin >> r >> c >> ee;
        r--, c--;
        e[r][c] = ee;
    }
    bool ans = false;

    vector<vector<int>> flag(h, vector<int>(w, -1));

    function<void(int, int, int)> dfs = [&](int x, int y, int eng) -> void {
        if (a[x][y] == 'T') {
            ans = true;
            return;
        }

        if (ans) return;
        if (flag[x][y] >= eng) return;
        if (e[x][y] >= eng) {
            eng     = e[x][y];
            e[x][y] = 0;
        }

        flag[x][y] = eng;

        if (eng <= 0) return;

        rep(i, 4) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (a[nx][ny] == '#') continue;
            if (a[nx][ny] == 'T') {
                dfs(nx, ny, eng - 1);
                return;
            };

            dfs(nx, ny, eng - 1);
        }
    };

    dfs(sx, sy, 0);

    if (ans) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }   
}