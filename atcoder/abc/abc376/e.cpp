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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll t;
    cin >> t;

    rep(i, t) {
        ll n, k;
        cin >> n >> k;
        vec a(n);
        vec b(n);
        vector<pll> p;
        ll ans = 4e18;
        ll tmp = 0;
        ll sum = 0;

        rep(i, n) cin >> a[i];
        rep(i, n) cin >> b[i];

        rep(i, n) p.push_back({a[i], b[i]});

        so(p);

        // for (auto x : p) cout << x.ft << " ";
        // cout << endl;
        // for (auto x : p) cout << x.sd << " ";
        // cout << endl;
        priority_queue<ll> q;
        rep(j, k) {
            q.push(p[j].sd);
            sum += p[j].sd;
        }
            // cout << "f sum:" << sum << endl;

        ans = p[k - 1].ft * (sum);
        // cout << "f ans: " << ans << endl;
        reps(j, k, n) {
            ll M = q.top();
            // cout << "M: " << M << " sd: " << p[j].sd << endl;
            q.pop();
            sum = sum - M + p[j].sd;
            // cout << "sum:" << sum << endl;
            q.push(p[j].sd);
            tmp = p[j].ft * sum;
            // cout << "tmp: " << tmp << endl;
            chmin(ans, tmp);
        }

        cout << ans << endl;
    }
}