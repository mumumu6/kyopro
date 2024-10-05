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

double dist(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, s, t;
    cin >> n >> s >> t;
    vecc a(n, vec(4));

    long double base = 0;

    vec p(n);

    rep(i, n) p[i] = i;

    rep(i, n) {
        rep(j, 4) cin >> a[i][j];
        base += dist(a[i][0], a[i][1], a[i][2], a[i][3]);
    }
    base /= (long double)(t);
    long double x   = 4e18;
    long double now = 0;
    ll s1, s2, g1, g2;
    do {
        rep(bit, (1 << n)) {
            now = 0.0;
            rep(i, n - 1) {
                if (bit & (1 << i)) {
                    s1 = 0;
                    g1 = 2;
                } else {
                    s1 = 2;
                    g1 = 0;
                }
                if (bit & (1 << (i + 1))) {
                    s2 = 0;
                    g2 = 2;
                } else {
                    s2 = 2;
                    g2 = 0;
                }

                if (i == 0) {
                    // 最初の点からの距離
                    now += dist(0, 0, a[p[i]][s1], a[p[i]][s1 + 1]);
                }

                now += dist(a[p[i]][g1], a[p[i]][g1 + 1], a[p[i + 1]][s2], a[p[i + 1]][s2 + 1]);
            }
            chmin(x, now);
        }
    } while (next_permutation(p.begin(), p.end()));
                                                                                                           
    cout << x / (long double)(s) + base << endl;
}