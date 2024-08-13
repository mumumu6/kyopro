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

class segment_tree {
  private:
    int sz;
    vector<int> seg;
    vector<int> lazy;
    void push(int k) {
        if (k < sz) {
            lazy[k * 2]     = max(lazy[k * 2], lazy[k]);
            lazy[k * 2 + 1] = max(lazy[k * 2 + 1], lazy[k]);
        }
        seg[k]  = max(seg[k], lazy[k]);
        lazy[k] = 0;
    }

    void update(int a, int b, int x, int k, int l, int r) {
        push(k);
        if (r <= a || b <= l) return;
        if (a <= l && r <= b) {
            lazy[k] = x;
            push(k);
            return;
        }
        update(a, b, x, k * 2, l, (l + r) >> 1);
        update(a, b, x, k * 2 + 1, (l + r) >> 1, r);
        seg[k] = max(seg[k * 2], seg[k * 2 + 1]);
    }

    int range_max(int a, int b, int k, int l, int r) {
        push(k);
        if (r <= a || b <= l) return 0;
        if (a <= l && r <= b) return seg[k];
        int lc = range_max(a, b, k * 2, l, (l + r) >> 1);
        int rc = range_max(a, b, k * 2 + 1, (l + r) >> 1, r);
        return max(lc, rc);
    }

  public:
    segment_tree() : sz(0), seg(), lazy() {};
    segment_tree(int N) {
        sz = 1;
        while (sz < N) {
            sz *= 2;
        }
        seg  = vector<int>(sz * 2, 0);
        lazy = vector<int>(sz * 2, 0);
    }
    void update(int l, int r, int x) { update(l, r, x, 1, 0, sz); }
    int range_max(int l, int r) { return range_max(l, r, 1, 0, sz); }

};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int W, N;
    cin >> W >> N;
    segment_tree seg(W);
    for (int i = 0; i < N; ++i) {
        int L, R;
        cin >> L >> R;
        int height = seg.range_max(L - 1, R) + 1;
        seg.update(L - 1, R, height);
        cout << height << '\n';
    }
    return 0;
}