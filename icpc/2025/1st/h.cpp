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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k;
    cin >> n >> k;

    vector<ll> a(n);
    vector<ll> p(n);
    vector<vector<ll>> g(n);

    rep(i, n) { cin >> a[i]; }
    rep(i, n - 1) {
        cin >> p[i + 1];
        p[i + 1]--;
        g[p[i + 1]].push_back(i + 1);
    }

    auto dfs = [&](auto dfs, ll x) -> vector<mint> {
        vector<mint> sum(k + 1, mint(0));
        if (a[x] <= k) sum[a[x]]++;
        
        for (ll to : g[x]) {
            // cerr << to << "\n";
            vector<mint> v = dfs(dfs, to);
            sum            = atcoder::convolution<mint>(sum, v);
            sum.resize(k + 1);
        }
        sum[0] = 1;

        // cout << "x = " << x << endl;
        // for (auto kk : sum) { cout << kk.val() << " "; }
        // cout << endl;
        return sum;
    };

    vector<mint> sum = dfs(dfs, 0);

    cout << sum[k].val() << endl;
}