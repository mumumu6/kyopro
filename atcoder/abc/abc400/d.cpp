#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'
#define debug(x) cerr << #x << " = " << x << el

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll h, w;
    cin >> h >> w;

    vector<string> s(h);
    rep(i, h) cin >> s[i];

    priority_queue<pair<ll, pll>, vector<pair<ll, pll>>, greater<pair<ll, pll>>> pq;

    vecc dist(h, vector<ll>(w, INF));

    ll a, b, c, d;
    cin >> a >> b >> c >> d;
    a--, b--, c--, d--;

    dist[a][b] = 0;
    pq.push({0, {a, b}});

    while (!pq.empty()) {
        auto [cost, pos] = pq.top();
        pq.pop();

        auto [x, y] = pos;

        if (dist[x][y] < cost) continue;

        rep(i, 4) {
            ll nx = x + dx[i];
            ll ny = y + dy[i];

            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (s[nx][ny] == '.') {
                if (dist[nx][ny] > cost) {
                    dist[nx][ny] = cost;
                    pq.push({cost, {nx, ny}});
                }
            }else {
                if (dist[nx][ny] > cost + 1) {
                    dist[nx][ny] = cost + 1;
                    pq.push({cost + 1, {nx, ny}});
                }

                ll nx2 = x + dx[i] * 2;
                ll ny2 = y + dy[i] * 2;

                if (nx2 < 0 || nx2 >= h || ny2 < 0 || ny2 >= w) continue;
                
                if (dist[nx2][ny2] > cost + 1) {
                    dist[nx2][ny2] = cost + 1;
                    pq.push({cost + 1, {nx2, ny2}});
                }
            }
        }
    }

    cout << dist[c][d] << el;
}