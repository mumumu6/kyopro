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
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

class UnionFind {
  public:
    vector<int> par;

    void init(int sz){  par.resize(sz, -1); }

    int root(int pos) {
        if (par[pos] == -1) return pos;
        return par[pos];
    }
    void unite(int u, int v) {
        u = root(u);
        v = root(v);
        if (u == v) return;
        par[u] = v;   
    }
    bool same(int u, int v) {
        if (root(u) == root(v)) return true;
        return false;
    }
};

int main() {
    UnionFind UF;
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n;
    vecc g(n,vec());
    vec a(n - 1), b(n - 1), c(n - 1);
    rep(i, n - 1) cin >> a[i] >> b[i] >> c[i];

    vec par(n,-1);
    

    rep(i, n - 1) {
        g[a[i]].pb(b[i]);
        g[b[i]].pb(a[i]);

        UF.unite(a[i], b[i]);
    }

    rep(i,n){
        if(g[i].size() == 1){
            
        }
    }

    
}