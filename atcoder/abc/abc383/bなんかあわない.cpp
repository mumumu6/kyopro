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
vector<ll> dx = {1, 0, -1, 0};
vector<ll> dy = {0, 1, 0, -1};
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
    ll ans = 0;

    vector<vector<char>> s(h, vector<char>(w));
    rep(i, h) rep(j, w) cin >> s[i][j];


    rep(x1, h) rep(y1, w) rep(x2, h) rep(y2, w) {
        if (x1 == x2 && y1 == y2) continue;
        if(s[x1][y1] == '#' || s[x2][y2] == '#') continue;

        vector<vector<bool>> visited(h, vector<bool>(w, false));

        queue<pll> q;
        q.push(mp(x1, y1));
        q.push(mp(x2, y2));
        // cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << endl;
        visited[x1][y1] = true; 
        visited[x2][y2] = true;
        ll tmp = 0;
        

        while(!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            tmp++;

            rep(i,4){
                ll nx = x + dx[i];
                ll ny = y + dy[i];
                if(abs(x1 - nx) + abs(y1 - ny) > d && abs(x2 - nx) + abs(y2 - ny) > d ) continue;

                if(nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
                if(s[nx][ny] == '#') continue;
                if(visited[nx][ny]) continue;

                visited[nx][ny] = true;
                q.push(mp(nx, ny));
                // cout << "nx: " << nx << " ny: " << ny << endl;
            }
        }


        chmax(ans, tmp);
    }

    cout << ans << endl;
}