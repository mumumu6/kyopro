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

    ll n, q;
    cin >> n >> q;

    vector<set<ll>> su(n);
    rep(i, n) su[i].insert(i);

    vec hato(n);
    rep(i, n) hato[i] = i;

    rep(i, q) {
        ll t;
        cin >> t;

        if (t == 1) {
            ll a, b;
            cin >> a >> b;
            a--;
            b--;

            ll bef  = hato[a];
            hato[a] = b;
            su[bef].erase(a);
            su[b].insert(a);
        } else if (t == 2) {
            ll a, b;
            cin >> a >> b;
            a--;
            b--;

            set<ll> tmp = su[a];
            su[a]       = su[b];
            su[b]       = tmp;

            for (auto x : su[a]) { hato[x] = a; }
            for (auto x : su[b]) { hato[x] = b; }

        } else {
            ll a;
            cin >> a;
            a--;

            cout << hato[a] + 1 << endl;
        }
    }
}