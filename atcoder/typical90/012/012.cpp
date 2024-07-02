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

struct UnionFind {
    vector<int> par, rank, siz;

    UnionFind(int n) { init(n); }

    void init(int n) {
        par.assign(n, -1);
        rank.assign(n, 0);
        siz.assign(n, 1);
    }

    int root(int x) {
        if (par[x] == -1) return x;
        else return par[x] = root(par[x]);
    }

    void unite(int u, int v) {
        int u_root = root(u);
        int v_root = root(v);

        if (u_root == v_root) return;

        if (rank[u_root] < rank[v_root]) swap(u_root, v_root); // vのrankが小さい
        par[v_root] = u_root;

        if (rank[u_root] == rank[v_root]) rank[u_root]++;
        siz[u_root] += siz[v_root];
    }

    int size(int x) { return siz[root(x)]; }

    bool same(int x, int y) { return root(x) == root(y); }
};

int main() {
    int h, w, q;
    cin >> h >> w >> q;
    UnionFind uf(h * w); // UnionFindのインスタンス化はh*wで行う

    vector<vector<bool>> red(h, vector<bool>(w, false));

    auto que1 = [&](int x, int y) {
        red[x][y] = true;
        vector<pair<int, int>> d = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        rep(i, 4) {
            int nx = x + d[i].ft;
            int ny = y + d[i].sd;
            if (0 <= nx && nx < h && 0 <= ny && ny < w) {
                if (red[nx][ny]) uf.unite(x * w + y, nx * w + ny);
            }
        }
    };

    auto que2 = [&](int x1, int y1, int x2, int y2) {
        return uf.same(x1 * w + y1, x2 * w + y2);
    };

    rep(i, q) {
        int t;
        cin >> t;
        if (t == 1) {
            int r, c;
            cin >> r >> c;
            r--;
            c--;
            que1(r, c);
        } else {
            int ra, ca, rb, cb;
            cin >> ra >> ca >> rb >> cb;
            ra--;
            ca--;
            rb--;
            cb--;

            if (que2(ra, ca, rb, cb)) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
    }

    return 0;
}
