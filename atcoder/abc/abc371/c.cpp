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
    int mg;
    cin >> mg;

    vector<vector<bool>> g(n, vector<bool>(n, false)), h(n, vector<bool>(n, false));

    rep(i, mg) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        g[a][b] = true;
        g[b][a] = true;
    }

    int mh;
    cin >> mh;
    rep(i, mh) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        h[a][b] = true;
        h[b][a] = true;
    }

    vector<vector<ll>> a(n, vector<ll>(n, 0));

    rep(i, n) reps(j, i + 1, n) {
        cin >> a[i][j];
        a[j][i] = a[i][j];

    }

    ll ans = 4e18;

    vector<ll> p(n);
    rep(i, n) p[i] = i;
    do {
        ll sum = 0;
        rep(i, n) rep(j, i) {
            if (h[i][j] != g[p[i]][p[j]]) {
                sum += a[i][j];
            }
        }
        chmin(ans, sum);
    } while (next_permutation(p.begin(), p.end()));

    cout << ans << endl;
    return 0;
}