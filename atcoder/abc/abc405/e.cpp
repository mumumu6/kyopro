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

    ll a, b, c, d;
    cin >> a >> b >> c >> d;



    mint ans = 0;

    vector<mint> fact(10101010, 1);
    vector<mint> fact_inv(10101010, 1);

    rep(i, 10101010) {
        if (i > 0) fact[i] = fact[i - 1] * i;
        fact_inv[i] = fact[i].inv();
    }

    auto nCr = [&](ll n, ll r) -> mint {
        if (n < r) return mint(0);
        if (n == r) return mint(1);
        return fact[n] * fact_inv[r] * fact_inv[n - r];
    };


    rep(i, c + 1) { // dの左のcの個数
        mint t = nCr(d + c - i - 1, d - 1);

        t *= nCr(a + i + b, b);
        ans += t;

        // cout << "i: " << i << " t: " << t.val() << endl;
    }

    cout << ans.val() << endl;
}