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

    ll n;
    cin >> n;
    vec a(n);
    rep(i, n) cin >> a[i];
    ll cnt = 2;
    ll d   = a[1] - a[0];
    ll ans = 0;

    if( n == 1){
        cout << 1 << endl;
        return 0;
    }else if (n == 2){
        cout << 3 << endl;
        return 0;
    }

    

    reps(i, 2, n) {
        if (a[i] - a[i - 1] == d)
            cnt++;
        else {
            if (ans == 0)
                ans += cnt * (cnt + 1) / 2;
            else
                ans += cnt * (cnt + 1) / 2 - 1;
            cnt = 2;
            d   = a[i] - a[i - 1];
        }

        if (i == n - 1) {
            if (ans == 0)
                ans += cnt * (cnt + 1) / 2;
            else
                ans += cnt * (cnt + 1) / 2 - 1;
        }
    }

    cout << ans << endl;
}