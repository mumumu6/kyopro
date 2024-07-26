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
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int h, w, y;
    cin >> h >> w >> y;

    vecc a(h, vec(w));
    rep(i, h) rep(j, w) cin >> a[i][j];
    ll s = h * w;
    vector<vector<bool>> sea(h, vector<bool>(w, false));
    rep(i, w) sea[0][i]     = true;
    rep(i, w) sea[h - 1][i] = true;
    rep(i, w) sea[i][0]     = true;
    rep(i, w) sea[i][w - 1] = true;

    int hight = 0;
    rep(i, y) {
        hight ++;

        rep(x,h)rep(y,w){
            if(!sea[x][y])continue;
            
        }
    }
}