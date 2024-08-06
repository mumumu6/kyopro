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

    ll n, m;
    ll cnts = 0, ans = 0;
    cin >> n >> m;
    vecc g(n), gg(n);
    stack<ll> back;
    vector<bool> used(n, false);

    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].pb(b);
        gg[b].pb(a);
    }

    function<void(ll)> dfs1 = [&](ll u) {
        used[u] = true;
        for (auto x : g[u]) {
            if (!used[x]) dfs1(x);
        }
        back.push(u);
    };
    function<void(ll)> dfs2 = [&](ll pos) {
        used[pos] = true;
        cnts++;
        for (int i : gg[pos]) {
            if (used[i] == false) dfs2(i);
        }
    };

    rep(i, n) {
        if (!used[i]) dfs1(i);
    }
    fill(all(used), false);

    while (!back.empty()) {
        ll v = back.top();
        back.pop();
        cnts = 0;

        dfs2(v);
        ans += cnts * (cnts - 1LL) / 2LL;
    }

    cout << ans << endl;
}