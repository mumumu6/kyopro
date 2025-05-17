#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'
#define debug(x) cerr << #x << " = " << x << el

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll h, w, n;
    cin >> h >> w >> n;

    vector<set<ll>> row(h), col(w);

    rep(i, n) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        row[a].insert(b);
        col[b].insert(a);
    }

    ll q;
    cin >> q;

    rep(qi, q) {
        ll t;
        cin >> t;

        if (t == 1) {
            ll x;
            cin >> x;

            x--;

            cout << row[x].size() << el;
            // cerr << "row = " << x << el;
            for (auto it : row[x]) {
                col[it].erase(x);
                // cerr << "delete = " << it << " ";
            }
            row[x].clear();
            // cerr << el;


        } else if (t == 2) {
            ll y;
            cin >> y;

            y--;

            cout << col[y].size() << el;
            // cerr << "col = " << y << el;
            for (auto it : col[y]) {
                row[it].erase(y);
                // cerr << "delete = " << it << " ";
            }
            col[y].clear();
            // cerr << el;
        }
    }
}