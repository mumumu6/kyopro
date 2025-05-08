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

    ll n, m, q;
    cin >> n >> m >> q;

    vector<set<ll>> s(n);
    vector<bool> used(n, false);

    rep(i, q) {
        ll t;
        cin >> t;

        if (t == 1) {
            ll x, y;
            cin >> x >> y;
            x--, y--;

            s[x].insert(y);
        } else if (t == 2) {
            ll x;
            cin >> x;
            x--;

            used[x] = true;
        } else {
            ll x, y;
            cin >> x >> y;
            x--, y--;

            if (used[x]) {
                cout << "Yes" << endl;
                continue;
            } else if (s[x].count(y)) {
                cout << "Yes" << endl;
                continue;
            } else {
                cout << "No" << endl;
            }
        }
    }
}