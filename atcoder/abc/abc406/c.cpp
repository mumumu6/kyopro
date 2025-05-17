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

    ll n;
    cin >> n;

    vec p(n);
    rep(i, n) cin >> p[i];

    ll ans = 0;

    vector<pll> c;
    c.push_back({-1, 1});

    reps(i, 2, n) {
        ll d  = p[i - 1] - p[i - 2];
        ll d2 = p[i] - p[i - 1];

        if ((d > 0 && d2 < 0) || (d < 0 && d2 > 0)) {
            c.push_back({i - 2, i});
        }
    }

    c.push_back({n - 2, n });

    ll siz = c.size();

    reps(i, 1, siz - 2) {
        ll tmp = 0;

        if(p[(c[i].ft + c[i].sd)/2 ] < p[(c[i + 1].ft + c[i + 1].sd) / 2]) continue;
        


        // cerr << "-----------------" << el;
        // cerr << "c[i] = " << c[i].ft << " " << c[i].sd << el;
        // cerr << "c[i - 1] = " << c[i - 1].ft << " " << c[i - 1].sd << el;
        // cerr << "c[i + 1] = " << c[i + 1].ft << " " << c[i + 1].sd << el;
        // cerr << "c[i + 2] = " << c[i + 2].ft << " " << c[i + 2].sd << el;

        tmp +=   (c[i].ft)  - (c[i - 1].ft + c[i - 1].sd) / 2 + 1;
        // cerr << "tmp = " << tmp << el;
        tmp *= (c[i + 2].ft + c[i + 2].sd) / 2 - (c[i + 1].sd) + 1 ;
        // cerr << "tmp = " << (c[i + 2].ft + c[i + 2].sd) / 2 - (c[i + 1].sd) + 1 << el;

        // cerr << tmp << el;

        ans += tmp;
    }

    cout << ans << el;
}