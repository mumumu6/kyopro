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

    map<ll, vector<pll>> extra; // 余分なケーブルを持ってるサーバーをメモ

    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;

        if (uf.same(a, b)) extra[uf.leader(a)].push_back(mp(a, i + 1));
        else {
            vector<pll> p = extra[uf.leader(a)];
            uf.merge(a, b);

            extra[uf.leader(a)].insert(extra[uf.leader(a)].end(), p.begin(), p.end());
        }
    }

    ll ans = 0;
    cout << uf.groups().size() - 1 << endl;

    vec haveno;
    auto groups = uf.groups();

    for (auto g : groups) {
        if (extra.find(uf.leader(g[0])) != extra.end()) haveno.pb(g[0]);
    }

    ll x = 0;
    for (auto g : groups) {
        if (extra.find(uf.leader(g[0])) != extra.end()) continue;

        if (x != haveno.size()) {
            cout << extra[uf.leader(g[0])].sd << " " << extra[uf.leader(g[0])].ft + 1  << " "
                 << uf.leader(haveno[x]) + 1<< endl;
            uf.merge(uf.leader(g[0]), uf.leader(haveno[x]));
            x++;    

        }
    }

    rep(i, groups.size() - 1) {
        cout << extra[uf.leader(groups[i][0])].sd << " " << extra[uf.leader(groups[i][0])].ft + 1 << " " << uf.leader(groups[i + 1][0]) + 1 << endl;
    }
}