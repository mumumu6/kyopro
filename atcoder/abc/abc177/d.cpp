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

struct UnionFind {
    vector<ll> par, siz;

    void init(int n) {
        par.resize(n, -1);
        siz.resize(n, 1);
    }

    int root(int x) {
        if (par[x] == -1) return x;
        return par[x] = root(par[x]);
    }

    void unite(int u, int v) {
        int u_root = root(u);
        int v_root = root(v);

        if (u_root == v_root) return;
        par[u_root] = v_root;
        siz[v_root] += siz[u_root];
    }

    bool same(int u, int v) { return root(u) == root(v); }

    int size(int x){
        return siz[root(x)];
    }
};

int main() {
    UnionFind UF;
    int n, m;
    cin >> n >> m;

    UF.init(n);

    rep(i, m) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        UF.unite(a, b);
    }

    set<int> a;

    rep(i, n) {
        a.insert(UF.root(i));
        // cout << UF.root(i) << endl;
    }
    int ans = 0;
    for(auto c:a){
        ans = max(ans,UF.size(c));
        // cout << UF.size(c) << endl;
    }

    cout <<ans << endl;
}