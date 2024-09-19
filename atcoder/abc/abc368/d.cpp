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

    ll n, k;
    cin >> n >> k;
    vector<set<ll>> g(n);

    rep(i, n - 1) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        g[a].insert(b);
        g[b].insert(a);
    }

    vector<bool> v(n, false);
    rep(i, k) {
        int a;
        cin >> a;
        a--;
        v[a] = true;
    }

    ll ans = n;

    function<void(ll)> f = [&](ll a) {
        if (v[a] || g[a].size() != 1) return;

        ans--;
        v[a]        = true;
        ll neighbor = *g[a].begin(); // 唯一の隣接ノードを取得

        g[neighbor].erase(a); // 隣接ノードから自ノードを削除
        g[a].clear();         // 自ノードのセットをクリア

        f(neighbor); // 隣接ノードで再帰的に処理
    };

    rep(i, n) { f(i); }

    cout << ans << endl;
}