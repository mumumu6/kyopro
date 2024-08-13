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
#define veccc vector<vector<vector<ll>>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n;
    cin >> n;
    veccc a(n + 1, vecc(n + 1, vec(n + 1)));

    rep(i, n) {
        rep(j, n) {
            rep(k, n) { cin >> a[i + 1][j + 1][k + 1]; }
        }
    }

    veccc sum(n + 1, vecc(n + 1, vec(n + 1, 0)));

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            for (int k = 1; k < n; k++) {
                sum[i][j][k] = a[i][j][k] + sum[i - 1][j][k] +
                               sum[i][j - 1][k] + sum[i][j][k - 1] -
                               sum[i - 1][j - 1][k] - sum[i - 1][j][k - 1] -
                               sum[i][j - 1][k - 1] + sum[i - 1][j - 1][k - 1];
            }
        }
    }

    int q;
    cin >> q;

    rep(i, q) {
        int lx, rx, ly, ry, lz, rz;
        cin >> lx >> rx >> ly >> ry >> lz >> rz;

        int result = sum[rx][ry][rz] - sum[lx - 1][ry][rz] -
                     sum[rx][ly - 1][rz] - sum[rx][ry][lz - 1] +
                     sum[lx - 1][ly - 1][rz] + sum[lx - 1][ry][lz - 1] +
                     sum[rx][ly - 1][lz - 1] - sum[lx - 1][ly - 1][lz - 1];

        cout << result << endl;
    }

    rep(i, n) rep(j, n) rep(k, n) cout << i << " " << j << " " << j
                                       << " " <<  sum[i][j][k] << endl;
}