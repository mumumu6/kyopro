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
#define vec vector<int>
#define vecc vector<vector<int>>

struct UnionFind {
    vec par;
    void init(int n) { par.resize(n, -1); }

    int root(int x) {
        if (par[x] == -1) return x;
        return par[x] = root(par[x]);
    }

    void unite(int u, int v) {
        int ur = root(u);
        int vr = root(v);

        if (ur == vr) return;
        par[ur] = vr;
    }

    bool same(int x, int y) { return root(x) == root(y); }
};

int main() {
    UnionFind UF;
    int n, m;
    cin >> n >> m;
    UF.init(n);
    int check = 0;
    vecc pas(n, vec());
    rep(i, m) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        pas[a].pb(b);
        pas[b].pb(a);

        UF.unite(a, b);
    }

    rep(i, n) {
        if (pas[i].size() == 1) {
            check++;
            if (check > 2) {
                cout << "No" << endl;
                return 0;
            }
            continue;
        } else if (pas[i].size() == 2) {
            continue;
        }
        cout << "No" << endl;
        return 0;
    }
    if (check != 2) {
        cout << "No" << endl;
        return 0;
    }
    int a = UF.root(0);
    rep(i, n) {
        if (a == UF.root(i)) continue;
        cout << "No" << endl;
        return 0;
    }

    cout << "Yes" << endl;
}