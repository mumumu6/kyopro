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

    ll l, r;
    cin >> l >> r;
    auto powi = [&](ll a, ll b) { // a^b
        ll ans = 1;
        rep(i, b) ans *= a;
        return ans;
    };

    auto g = [&](ll x, ll y, auto g) -> ll { // 999
        if (x == 0) return 1;
        // cout << "x: " << x << endl;

        ll x_digit = 0;
        ll x_tmp   = x;
        while (x_tmp > 0) {
            x_tmp /= 10;
            x_digit++;
        }

        ll lx = x;
        // 各桁をベクターに格納
        vector<ll> x_vec(x_digit);
        rep(i, x_digit) x_vec[i] = lx % 10, lx /= 10; // 78453

        reverse(all(x_vec));

        if(x_vec[0] >= y){
            ll ans = powi(y, x_digit);

            return ans;
        }

        ll tmp = x_vec[0];
        // cout << "tmp: " << tmp << endl;
        // cout << "y: " << y << endl;
        // cout << "x_vec[1]: " << x_vec[0] << endl;

        rep(i, x_digit - 1) tmp *= y;
        ll ans = tmp;
        // cout << "ans: " << ans << endl;

        ll nx = x - x_vec[0] * powi(10, x_digit - 1);

        ans += g(nx, y, g);
        // cout << "ans: " << ans << endl;
        // cout << "nx: " << nx << endl;
        return ans;
    };

    auto f = [&](ll x, auto f) -> ll {
        // x の桁数を求める
        if (x < 10) return 0;
        ll x_digit = 0;
        ll x_tmp   = x;
        while (x_tmp > 0) {
            x_tmp /= 10;
            x_digit++;
        }

        ll lx = x;
        // 各桁をベクターに格納
        vector<ll> x_vec(x_digit);
        rep(i, x_digit) x_vec[i] = lx % 10, lx /= 10;

        reverse(all(x_vec));

        ll ans = 0;
        rep(i, x_digit - 1) {
            if (i == 0) continue;
            rep(j, 9) ans += powi(j + 1, i);
        }
        // cout << "koko" << endl;
        // cout << "ans: " << ans << endl;

        rep(i, x_vec[0]) {
            if (i == 0) continue;
            ans += powi(i, x_digit - 1);
        }
        // cout << "ans: " << ans << endl;

        ll nx = x - x_vec[0] * powi(10, x_digit - 1);

        ans += g(nx, x_vec[0], g);
        // cout << "ans: " << ans << endl;

        return ans;
    };

    cout  << f(r, f)  - f(l-1,f)<< endl;
} // 45  + 1 + 4 + 9 + 81 = 285 + 45  + 1 = 331

// 9 * 10 * 19 / 6 = 285

// 330 + 1 + 8 + 27 + 9^^3 = 330 + 2025 = 2355
