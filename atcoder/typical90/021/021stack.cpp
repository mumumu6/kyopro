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
    vecc g(n), gg(n);
    vector<bool> used(n, false);
    rep(i, m) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].pb(b);
        gg[b].pb(a);
    }

    stack<ll> s, back;
    rep(i, n) {
        if (used[i]) continue;

        s.push(~i);
        s.push(i);
        while (!s.empty()) {
            ll x = s.top();
            s.pop();

            if (x >= 0) {
                used[x] = true;

                for (auto u : g[x]) {
                    if (!used[u]) {
                        s.push(~u);
                        s.push(u);
                    }
                }
            } else {
                back.push(~x);
            }
        }
    }

    fill(all(used), false);
    ll ans = 0;

    while (!back.empty()) {

        ll v = back.top();
        back.pop();

        if (used[v]) continue;
        stack<ll> ss;
        ss.push(v);
        ll cnt = 1;
        while (!ss.empty()) {
            ll u = ss.top();
            ss.pop();
            if (!used[u]) {

                used[u] = true;
                for (auto x : gg[u]) {
                    if (!used[x]) {
                        cnt++;
                        ss.push(x);
                    }
                }
            }
        }

        ans += cnt * (cnt - 1LL) / 2LL;
    }

    cout << ans << endl;
}