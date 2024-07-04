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

class UnionFind {
  public:
    vector<int> par, size;

    void init(int sz) { par.resize(sz, -1); }

    int root(int pos) {
        if (par[pos] == -1) return pos;
        return par[pos] = root(par[pos]);
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
UnionFind UF;

int main() {
    int n, m;
    cin >> n >> m;

    UF.init(n);

    rep(i, m) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        UF.unite(u, v);
    }

    set<int> ans;
    rep(i, n) { ans.insert(UF.root(i)); }

    cout << ans.size() << endl;
}