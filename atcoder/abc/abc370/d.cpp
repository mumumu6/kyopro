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

    ll h, w, q;
    cin >> h >> w >> q;

    vector<set<int>> g1(h), g2(w);

    rep(i, h) rep(j, w) {
        g1[i].insert(j);
        g2[j].insert(i);
    }

    auto erase = [&](int i, int j) {
        g1[i].erase(j);
        g2[j].erase(i);
    };

    rep(i, q) {
        ll r, c;
        cin >> r >> c;
        r--, c--;

        if(g1[r].find(c) != g1[r].end()) {
            erase(r, c);
            continue;
        }

        // 上
        auto itr1 = g2[c].lower_bound(r);
        if(itr1 != g2[c].begin())erase(*--itr1, c);
        auto itr2 = g2[c].lower_bound(r);
        if (itr1 != g2[c].end()) erase(*itr2, c);

        auto itr =  g1[r].lower_bound(c);
        if(itr != g1[r].begin()) erase(r, *--itr);
        auto itr3 = g1[r].lower_bound(c);
        if(itr != g1[r].end()) erase(r, *itr3);
        
    }



    int ans = 0;
    rep(i,h) ans += g1[i].size();

    cout << ans <<'\n';
}