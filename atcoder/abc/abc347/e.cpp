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

    ll n, q;
    cin >> n >> q;
    vec x(q);
    rep(i, q) {
        cin >> x[i];
        x[i]--;
    }

    set<ll> s;

    vec sum(q + 1,0);

    rep(i, q) {
        if (s.find(x[i]) != s.end()) {
            s.erase(x[i]);
            sum[i + 1] = sum[i] + s.size();
        } else {
            s.insert(x[i]);
            sum[i + 1] = sum[i] + s.size();
        }
    }

    vecc a(n, vec());

    rep(i, q) a[x[i]].pb(i);
    rep(i, n) {
        if (a[i].size() % 2 == 1) {
            a[i].pb(q);
        }
    }

    vec ans(n,0);

    rep(i, n) {
        ll siz = a[i].size();
        ll tim = siz / 2;

        rep(j, tim){
            ll in = a[i][2*j];
            ll out = a[i][2*j+1];

            ans[i] += sum[out] - sum[in];
        }


    }

    rep(i, n) cout << ans[i] << " ";
    cout << endl;
    return 0;
}