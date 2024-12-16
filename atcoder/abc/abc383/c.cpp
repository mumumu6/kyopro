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

    ll h, w, d;
    cin >> h >> w >> d;
    ll ans = 0;

    vector<vector<char>> s(h, vector<char>(w));
    vector<vector<ll>> dist(h, vector<ll>(w, -1));

    priority_queue<pair<ll, pll>> q;
    rep(i, h) rep(j, w) {
        cin >> s[i][j];
        if (s[i][j] == 'H') {
            q.push(mp(d, mp(i, j)));
            dist[i][j] = d;
            ans++;
        }
    }
    vector<vector<char>> ss = s;

    while (!q.empty()) {

        auto [cost, p] = q.top();
        q.pop();
        if (cost <= 0) continue;
        auto [x, y] = p;

        rep(i, 4) {
            ll nx = x + dx[i];
            ll ny = y + dy[i];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (s[nx][ny] == '#') continue;

            if (dist[nx][ny] >= cost - 1) continue;
            if (dist[nx][ny] == -1) ans++;

            // ss[nx][ny] = 'o';

            dist[nx][ny] = cost - 1;
            q.push(mp(cost - 1, mp(nx, ny)));
        }
    }

    cout << ans << endl;

    // for (auto i : ss) {
    //     for (auto j : i) { cout << j; }
    //     cout << endl;
    // }
}