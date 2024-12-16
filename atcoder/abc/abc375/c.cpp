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

    ll n;
    cin >> n;

    vector<vector<char>> a(n, vector<char>(n));
    vector<vector<char>> b(n, vector<char>(n));
    rep(i, n) rep(j, n) cin >> a[i][j];

    rep(nx,n)rep(ny,n){
		ll z = min({nx + 1, ny + 1, n - nx, n - ny});
		ll x = nx, y = ny;

		rep(j,z % 4){
			ll tx = x;
			ll ty = y;
			x = ty;
			y = n  -1  - tx;
		}

		b[x][y] = a[nx][ny];
	}



	rep(i,n)rep(j,n){
		cout << b[i][j] ;
		if(j == n-1) cout << endl;
	}
}