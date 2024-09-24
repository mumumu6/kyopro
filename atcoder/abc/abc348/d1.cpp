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

struct Med {
    ll r, c, e;
    Med() {}
    Med(int r, int c, int e) : r(r), c(c), e(e) {}
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    vec dx = {1, 0, -1, 0};
    vec dy = {0, 1, 0, -1};

    ll h, w, sx, sy, tx, ty;
    cin >> h >> w;
    vector<vector<char>> a(h, vector<char>(w));
    rep(i, h) rep(j, w) {
        cin >> a[i][j];

        if (a[i][j] == 'S') {
            sx = i;
            sy = j;
        } else if (a[i][j] == 'T') {
            tx = i;
            ty = j;
        }
    }

    ll n;
    cin >> n;

    vector<Med> meds(n);

    rep(i, n) {
        ll r, c, e;
        cin >> r >> c >> e;
        r--, c--;
        meds[i] = Med(r, c, e);
    }

    meds.emplace_back(sx, sy, 0);
    meds.emplace_back(tx, ty, 0);
    n += 2;

    ll inf = 4e18;

    vecc to(n, vec());
    rep(i, n) {
        auto [r1, c1, e1] = meds[i];
        vecc dist(h, vec(w, inf));
        queue<pll> que;
        dist[r1][c1] = 0;
        que.emplace(r1, c1);

        while (!que.empty()) {
            auto [i, j] = que.front();
            que.pop();
            rep(dir, 4) {
                int ni = i + dx[dir], nj = j + dy[dir];
                if (ni < 0 || nj < 0 || ni >= h || nj >= w) continue;
                if (a[ni][nj] == '#') continue;
                if (dist[ni][nj] != inf) continue;
                dist[ni][nj] = dist[i][j] + 1;
                que.emplace(ni, nj);
            }
        }

        rep(v, n) if (i != v) {
            auto [r, c, e] = meds[v];
            if (dist[r][c] <= e1) to[i].push_back(v);
        }
    }

    queue<ll> q;
    q.push(n - 2);
    vector<bool> flag(n,false);
    flag[n-2] = true;
    while(!q.empty()){
        ll now = q.front();
        q.pop();

        for(auto x : to[now]){
            if(flag[x]) continue;
            flag[x] = true;
            q.push(x);
        }
    }

    if(flag[n-1]){
        cout << "Yes" << endl;
        return 0;
    }else cout << "No" << endl;
}