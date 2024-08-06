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

    int h, w, si, sj;
    cin >> h >> w >> si >> sj;
    si--;
    sj--;
    vector<vector<char>> c(h, vector<char>(w));

    rep(i, h) rep(j, w) cin >> c[i][j];

    string x;
    cin >> x;
    int ni = si, nj = sj;

    rep(i, x.size()) {
        char a = x[i];

        if (a == 'U') {
            if (ni > 0 && c[ni - 1][nj] != '#') ni--;
        } else if (a == 'D') {
            if (ni < h - 1 && c[ni + 1][nj] != '#') ni++;
        } else if (a == 'L') {
            if (nj > 0 && c[ni][nj - 1] != '#') nj--;
        } else if (a == 'R') {
            if (nj < w - 1 && c[ni][nj + 1] != '#') nj++;
        }

        // cout << ni  << " " << nj << endl;
    }
    cout << ni + 1 << " " << nj + 1 << endl;
}