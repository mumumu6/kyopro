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
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n;
    cin >> n;
    vec a(n);
    vec b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];

    vector<pll> ab(2 * n);
    rep(i, n) {
        ab[i]     = mp(a[i], 0);
        ab[i + n] = mp(b[i], 1);
    }

    sort(all(ab));

    ll ans  = 0;
    int now = 0;
    int ai  = 0;
    int bi  = 0;

    vector<bool> used(2*n, false);
    rep(i, 2 * n) {
        auto [d, wh] = ab[i];
        if (used[i]) continue;
        used[i] = true;

        if (wh == 0) {
            while (used[bi] || ab[bi].sd == 0) bi++;
            used[bi] = true;
            ans += ab[bi].ft - d;
            // cout << "a b " << d << " " << ab[bi].ft << endl;
        }

        if (wh == 1) {
            while (used[ai] || ab[ai].sd == 1) {
                ai++;
                //  cout << ai << " " ;
            }
            // cout << endl;
            used[ai] = true;
            ans += ab[ai].ft - d;
            // cout << "b a " << d << " " << ab[ai].ft << endl;
        }
    }

    cout << ans << endl;
}