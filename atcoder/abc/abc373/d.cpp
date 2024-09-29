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

    ll n, m;
    cin >> n >> m;

    dsu uf(n);
    vector<vector<pll>> g(n);

    rep(i, m) {
        ll a, b, w;
        cin >> a >> b >> w;
        a--;
        b--;
        g[a].push_back(mp(b, w));
        g[b].push_back(mp(a, -w));

        uf.merge(a, b);
    }

    vector<ll> ans(n);

    for (auto x : uf.groups()) {
        stack<ll> st;
        vector<bool> used(n, false);

        st.push(x[0]);
        used[x[0]] = true;
        ans[x[0]]  = 0;

        while (!st.empty()) {
            ll now = st.top();
            st.pop();
            

            for (auto z : g[now]) { // ftが行先　sdが重み
                if (used[z.ft]) continue;
                ans[z.ft] = ans[now] + z.sd;
                st.push(z.ft);
                used[z.ft] = true;
            }
        }
    }

    rep(i, n) cout << ans[i] << " ";
    cout << endl;
}