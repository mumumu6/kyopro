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

    ll h, w, x, p, q;
    cin >> h >> w >> x >> p >> q;
    p--;
    q--;
    vector<vector<ll>> s(h, vector<ll>(w));
    rep(i, h) rep(j, w) cin >> s[i][j];

    // 小さい奴から取り出すpriority_queue
    priority_queue<pair<ll, pll>, vector<pair<ll, pll>>, greater<pair<ll, pll>>> que;

    ll ans = s[p][q];
    que.push(mp(s[p][q], mp(p, q)));
    bool flag = false;

    vector<vector<bool>> used(h, vector<bool>(w, false));
    used[p][q] = true;

    while (!que.empty()) {
        auto [cost, pp] = que.top();
        auto [a, b]     = pp;
        que.pop();
        if (flag && s[a][b] < static_cast<double>(ans) / x) {
            ans += s[a][b];

        } 
        if (flag && s[a][b] >= static_cast<double>(ans) / x) {
            continue;
        }
        rep(i, 4) {
            ll nx = a + dx[i];
            ll ny = b + dy[i];

            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (used[nx][ny]) continue;

            que.push(mp(s[nx][ny], mp(nx, ny)));
            used[nx][ny] = true;
        }
        flag = true;
    }

    cout << ans << endl;
}