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

    ll h, w;
    cin >> h >> w;

    const ll inf = 4e18;

    vector<vector<char>> s(h, vector<char>(w));

    vector<vector<char>> t(h, vector<char>(w));
    vector<vector<ll>> dist(h, vector<ll>(w, inf));

    // 距離、前の地点、現在地
    priority_queue<tuple<ll, pll, pll>, vector<tuple<ll, pll, pll>>, greater<tuple<ll, pll, pll>>> que;

    rep(i, h) {
        rep(j, w) {
            cin >> s[i][j];
            if (s[i][j] == 'E') {
                que.push({0, {-1, -1}, {i, j}});
                t[i][j]    = 'E';
                dist[i][j] = 0;
            } else if (s[i][j] == '#') {
                t[i][j] = '#';
            }
        }
    }

    while (!que.empty()) {
        auto [d, pre, now] = que.top();

        auto [px, py]     = pre;
        auto [nowx, nowy] = now;
        que.pop();

        if (s[nowx][nowy] == '#') continue;

        ll ddx = px - nowx;
        ll ddy = py - nowy;

        if (s[nowx][nowy] != 'E') {
            if (ddx == 1) {
                t[nowx][nowy] = 'v';
            } else if (ddx == -1) {
                t[nowx][nowy] = '^';
            } else if (ddy == 1) {
                t[nowx][nowy] = '>';
            } else if (ddy == -1) {
                t[nowx][nowy] = '<';
            }
        }

        rep(i, 4) {
            ll nx = nowx + dx[i];
            ll ny = nowy + dy[i];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (s[nx][ny] == '#') continue;
            if (s[nx][ny] == 'E') continue;

            if (dist[nx][ny] <= d + 1) continue;

            dist[nx][ny] = d + 1;

            que.push({d + 1, now, {nx, ny}});
            // cout << d << " prex = " << px << " prey = " << py << " nowx = " << nowx << " nowy = " << nowy
            //      << " dist[nx][ny] = " << dist[nx][ny] << endl;

            // if (nx == 0 && ny == 3) {
            //     cout << "nx = " << nx << " ny = " << ny << endl;
            //     cout << "dist[nx][ny] = " << dist[nx][ny] << endl;
            //     cout << "d = " << d + 1 << endl;
            //     cout << "px = " << px << " py = " << py << endl;
            //     cout << "nowx = " << nowx << " nowy = " << nowy << endl;
            //     cout << "t[nx][ny] = " << t[nx][ny] << endl;
            //     cout << "t[nowx][nowy] = " << t[nowx][nowy] << endl;
            //     cout << "t[px][py] = " << t[px][py] << endl;
            //     cout << "s[nx][ny] = " << s[nx][ny] << endl;
            //     cout << "s[nowx][nowy] = " << s[nowx][nowy] << endl;
            //     cout << "s[px][py] = " << s[px][py] << endl;
            //     cout << "dist[nx][ny] = " << dist[nx][ny] << endl;
            //     cout << "dist[nowx][nowy] = " << dist[nowx][nowy] << endl;
            // }
        }
    }

    for (auto &x : t) {
        for (auto &y : x) { cout << y; }
        cout << endl;
    }
}
