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

    dsu uf(n);

    vector<pll> extra; // 余分なケーブルか

    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;

        if (uf.same(a, b)) extra.push_back(mp(a, i + 1));
        else uf.merge(a, b);
    }

    auto g = uf.groups();
    cout << g.size() - 1 << endl;
    set<ll> merged;

    ll t = 0;
    ll mergedLeader;

    rep(i, extra.size()) {
        if (i == 0) {
            mergedLeader = uf.leader(extra[0].ft);
            merged.insert(mergedLeader);
        }

        auto [a, num] = extra[i];

        ll la = uf.leader(a);

        if (merged.count(la)) {
            while (t < g.size() && merged.count(uf.leader(g[t][0]))) t++;
            if (t >= g.size()) continue;

            cout << num << " " << a + 1 << " " << uf.leader(g[t][0]) + 1 << endl;
            merged.insert(uf.leader(g[t][0]));
            merged.insert(a);
        } else {
            cout << num << " " << a + 1 << " " << mergedLeader + 1 << endl;
            merged.insert(la);
            merged.insert(a);
        }

        if (i >= g.size() - 2) break;
    }
}