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
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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

    ll n, m, x, y;
    cin >> n >> m >> x >> y;
    x--, y--;

    map<ll, set<ll>> gx, gy;

    rep(i, n) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;

        gx[a].insert(b);
        gy[b].insert(a);
    }

    ll ans = 0;

    rep(i, m) {
        char d;
        ll c;
        cin >> d >> c;

        if (d == 'U') {

            if (gx.count(x) == 0) {
                y += c;
                continue;
            }
            auto it_lower = gx[x].lower_bound(y);
            auto it_upper = gx[x].upper_bound(y + c);

            for (auto it = it_lower; it != it_upper; it++) {
                gy[*it].erase(x); // 他の要素からxを削除
            }
            ans += distance(it_lower, it_upper); // 要素数をカウント
            gx[x].erase(it_lower, it_upper);     // 現在の要素を削除

            y += c;

        } else if (d == 'D') {
            if (gx.count(x) == 0) {
                y -= c;
                continue;
            }

            auto it_upper = gx[x].upper_bound(y);
            auto it_lower = gx[x].lower_bound(y - c);

            for (auto it = it_lower; it != it_upper; it++) {
                gy[*it].erase(x); // 他の要素からxを削除
            }

            ans += distance(it_lower, it_upper); // 要素数をカウント
            gx[x].erase(it_lower, it_upper);     // 現在の要素を削除

            y -= c;

        } else if (d == 'L') {
            if (gy.count(y) == 0) {
                x -= c;
                continue;
            }

            auto it_upper = gy[y].upper_bound(x);
            auto it_lower = gy[y].lower_bound(x - c);

            for (auto it = it_lower; it != it_upper; it++) {
                gx[*it].erase(y); // 他の要素からyを削除
            }

            ans += distance(it_lower, it_upper); // 要素数をカウント
            gy[y].erase(it_lower, it_upper);     // 現在の要素を削除

            x -= c;

        } else if (d == 'R') {
            if (gy.count(y) == 0) {
                x += c;
                continue;
            }

            auto it_lower = gy[y].lower_bound(x);
            auto it_upper = gy[y].upper_bound(x + c);

            for (auto it = it_lower; it != it_upper; it++) {
                gx[*it].erase(y); // 他の要素からyを削除
            }

            ans += distance(it_lower, it_upper); // 要素数をカウント
            gy[y].erase(it_lower, it_upper);     // 現在の要素を削除

            x += c;
        }
    }

    cout << x + 1 << " " << y + 1 << " " << ans << endl;
}
