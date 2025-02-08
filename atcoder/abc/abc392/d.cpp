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

    ll n;
    cin >> n;

    vector<set<ll>> g(n);
    vector<map<ll, ll>> cnt(n);
    vec k(n);

    rep(i, n) {
        cin >> k[i];
        rep(j, k[i]) {
            ll a;
            cin >> a;
            a--;
            g[i].insert(a);
            cnt[i][a]++;
        }
    }

    double ans = 0;

    rep(i, n) rep(j, i) {
        double t = 0;
        for (auto x : g[i]) {
            t += cnt[i][x] * cnt[j][x]; 
        }

    
        t /=static_cast<long double>(k[i] * k[j]);

        chmax(ans, t);
    }

    cout << ans << endl;
}