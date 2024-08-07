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

    ll n, m;
    cin >> n >> m;
    vecc g(n), gg(n);
    vector<bool> used(n, false);
    vector<bool> us(n, false);
    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].pb(b);
        gg[b].pb(a);
    }

    stack<ll> s, back;
    rep(i, n) {
        if (used[i]) continue;
        s.push(i);
        while (!s.empty()) {
            ll v = s.top();
            s.pop();

            if (v >= 0) {
                if (used[v]) continue; // 同じ頂点が複数個入力されてもここで弾く
                used[v] = true;
                s.push(~v); // ノードを訪問した後にスタックに戻す

                for (auto u : g[v]) {
                    if (!used[u]) {
                        s.push(u);
                    }
                }
            } else {
                back.push(~v); // 負の値を元に戻してスタックに追加
            }
        }
    }
    fill(all(used), false);
    ll ans                  = 0;
    ll cnt                  = 0;
    function<void(ll)> dfs2 = [&](ll pos) {
        used[pos] = true;
        cnt++;
        for (int i : gg[pos]) {
            if (used[i] == false) dfs2(i);
        }
    };
    while (!back.empty()) {
        ll v = back.top();
        back.pop();
        cnt = 0;
        if (used[v]) continue;
        dfs2(v);
        ans += cnt * (cnt - 1LL) / 2LL;
    }

    cout << ans << endl;
}