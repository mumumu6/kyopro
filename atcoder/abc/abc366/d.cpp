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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n;
    cin >> n;

    vector<vecc> a(n, vector<vector<ll>>(n, vector<ll>(n)));
    rep(i, n) rep(j, n) rep(k, n) cin >> a[i][j][k];

    int q;
    cin >> q;

    vector<vecc> s(n + 1, vecc(n + 1, vec(n + 1, 0)));

    rep(i, n) rep(j, n) rep(k, n) {
        s[i + 1][j + 1][k + 1] = s[i][j + 1][k + 1] + s[i + 1][j][k + 1] + s[i + 1][j + 1][k] 
        - s[i+1][j][k] - s[i][j+1][k] - s[i][j][k+1] + s[i][j][k] +  a[i][j][k];
    }

    rep(i,q){
        int lx,rx,ly,ry,lz,rz;
        cin >> lx >> rx >> ly >> ry >> lz >> rz;

        cout << s[rx][ry][rz] - s[rx][ry][lz-1] - s[rx][ly-1][rz] - s[lx-1][ry][rz] 
        + s[lx-1][ly-1][rz] + s[lx-1][ry][lz-1] + s[rx][ly-1][lz-1] - s[lx-1][ly-1][lz-1] << endl;
    }

    
}