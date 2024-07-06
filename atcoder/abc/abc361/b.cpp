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

bool has_positive_intersection(int a, int b, int c, int d, int e, int f, int g,
                               int h, int i, int j, int k, int l) {
    // Check for overlap in x, y, and z directions
    bool x_overlap = max(a, g) < min(d, j);
    bool y_overlap = max(b, h) < min(e, k);
    bool z_overlap = max(c, i) < min(f, l);

    return x_overlap && y_overlap && z_overlap;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int a, b, c, d, e, f, g, h, i, j, k, l;
    cin >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l;

    if (has_positive_intersection(a, b, c, d, e, f, g, h, i, j, k, l)) {
        cout << "Yes" << std::endl;
    } else {
        cout << "No" << std::endl;
    }
}