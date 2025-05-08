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

    ll n, m;
    cin >> n >> m;

    vec c(n);
    rep(i, n) cin >> c[i];

    vector<vector<ll>> a(m);   // i番目の動物がいる動物園のリスト
    vector<vector<ll>> zoo(n); // 動物園にどの動物がいるか

    rep(i, m) {
        ll k;
        cin >> k;
        a[i].resize(k);
        rep(j, k) {
            cin >> a[i][j];
            a[i][j]--;
            zoo[a[i][j]].push_back(i);
        }
    }

    vector<pair<ll, vector<ll>>> b;

    vector<ll> cnt(m, 0);

    b.push_back({0, cnt}); // cntはもう見た動物のカウント

    rep(i, m) { // i番目の動物に関して

        vector<pair<ll, vec>> new_b;
        for (auto &[cost, cnt] : b) {

            ll siz = a[i].size();

            if (cnt[i] == 0) {              // i番目の動物を一匹も見てなかったら
                rep(j, siz) rep(k, j + 1) { // i園とj園の動物園を選ぶ
                    vector<ll> tmp = cnt;

                    ll zoo_a = a[i][j];
                    ll zoo_b = a[i][k];
                    for (auto x : zoo[zoo_a]) tmp[x]++;
                    for (auto y : zoo[zoo_b]) tmp[y]++;

                    ll new_cost = cost + c[zoo_a] + c[zoo_b];

                    new_b.push_back({new_cost, tmp});
                }
            } else if (cnt[i] == 1) {
                rep(j, siz) {
                    vector<ll> tmp = cnt;

                    ll zoo_a = a[i][j];

                    for (auto x : zoo[zoo_a]) tmp[x]++;

                    ll new_cost = cost + c[zoo_a];

                    new_b.push_back({new_cost, tmp});
                }
            } else {
                new_b.push_back({cost, cnt});
            }
        }

        b = new_b;
    }

    ll ans = 4e18;

    for (auto [cost, cnt] : b) {
        bool ok                      = true;
        rep(i, m) if (cnt[i] < 2) ok = false;
        if (ok) chmin(ans, cost);
    }

    cout << ans << endl;
}   