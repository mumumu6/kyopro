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

    vector<vector<pll>> g(n);

    rep(i, n-1) {
        ll u, v, w;
        cin >> u >> v >> w;
        u--, v--;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    auto dfs = [&](auto dfs, ll v, ll p) -> pll {
        pll res = {0, 0};
        vec children_score;

        for (auto [to, cost] : g[v]) {
            if (to == p) continue;

            auto [contain, not_contain] = dfs(dfs, to, v);
            chmax(res.ft, contain + cost);
            children_score.pb(contain + cost);
            chmax(res.sd, not_contain);
        }

        sort(children_score.rbegin(), children_score.rend());

        if (children_score.size() >= 1) chmax(res.sd, children_score[0]);
        if (children_score.size() >= 2) chmax(res.sd, children_score[0] + children_score[1]);
        // cout << "v: " << v << " contain: " << res.ft << " not_contain: " << res.sd << endl;

        return res;
    };

    auto [contain, not_contain] = dfs(dfs, 0, -1);

    cout << not_contain << endl;
    return 0;
}