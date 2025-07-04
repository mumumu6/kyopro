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

    vector<pair<pll, vec>> q(m);

    dsu uf(n);

    rep(i, m) {
        cin >> q[i].ft.ft >> q[i].ft.sd;

        rep(j, q[i].ft.ft) {
            ll a;
            cin >> a;
            a--;
            q[i].sd.pb(a);
        };
    }

    sort(all(q), [](const pair<pll, vec> &a, const pair<pll, vec> &b) {
        return a.ft.sd < b.ft.sd;
    });

    set<ll> s;
    ll ans = 0;

    rep(i, m) {

        rep(j, q[i].ft.ft) {
            if (j == 0) continue;

            if (!uf.same(q[i].sd[0], q[i].sd[j])) {
                uf.merge(q[i].sd[0], q[i].sd[j]);
                ans += q[i].ft.sd;
            }
        }
    }

    if (uf.size(0) != n) {
        cout << -1 << endl;
        return 0;
    }
    cout << ans << endl;
}
