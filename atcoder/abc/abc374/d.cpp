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

long double dist(pll x, pll y) {
    long double dx = x.ft - y.ft;
    long double dy = x.sd - y.sd;
    return sqrtl(dx * dx + dy * dy);
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);
    ll n;
    long double s, t;
    cin >> n >> s >> t;
    vector<pair<long double, long double>> a(n);
    vector<pair<long double, long double>> b(n);

    long double base = 0;

    vec p(n);
    rep(i, n) p[i] = i;

    rep(i, n) {
        cin >> a[i].ft >> a[i].sd >> b[i].ft >> b[i].sd;
        base += dist(a[i], b[i]); // 線分の長さはここで計算
    }
    base /= (t);

    long double x   = 4e18; // 線分間の移動の距離の和
    long double now = 0;
    pair<long double, long double> s1, g1, s2, g2;
    do {
        rep(bit, (1 << n)) {
            now = 0.0;
            if (bit & (1 << 0)) s1 = a[p[0]];
            else s1 = b[p[0]];

            now += dist(make_pair(0.0, 0.0), s1); // 始点からスタート地点までの距離

            rep(i, n - 1) {

                if (bit & (1 << i)) { // bitがたってたらaがスタート
                    s1 = a[p[i]];
                    g1 = b[p[i]];
                } else {
                    s1 = b[p[i]];
                    g1 = a[p[i]];
                }
                if (bit & (1 << (i + 1))) {
                    s2 = a[p[i + 1]];
                    g2 = b[p[i + 1]];
                } else {
                    s2 = b[p[i + 1]];
                    g2 = a[p[i + 1]];
                }

                now += dist(g1, s2); // 線分間の移動距離
            }

            chmin(x, now);
        }
    } while (next_permutation(p.begin(), p.end()));

    x /= s;

    cout << x + base << endl;
}