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

    ll n, k;
    cin >> n >> k;

    function<ll(ll)> f8 = [&](ll x) {
        ll num = 0;
        ll t   = 1;
        while (x) {

            num += t * (x % 10);
            x /= 10;
            t *= 8;
        }
        return num;
    };
    auto f9 = [&](ll x){
        ll num = 0;
        ll t   = 1;
        while (x) {
            num += x % 9 * t;
            x /= 9;
            t *= 10;
        }
        return num;
    };

    function<ll(ll)> g = [&](ll x) {
        ll num = 0;
        ll t   = 1;
        while (x) {
            if (x % 10 == 8)
                num += 5 * t;
            else
                num += (x % 10) * t;

            t *= 10;
            x /= 10;
        }
        return num;
    };
    __int128 ans = __int128(n);
    rep(i, k) { ans = g(f9(f8(ans))); }
    ll y = ll(ans);
    cout << y << endl;
}