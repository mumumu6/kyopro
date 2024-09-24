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

    ll pow = 1;
    rep(i, 60) pow *= 2;

    auto f = [&](ll x) -> ll {
        if (x == 0) {
            return pow;
        }
        ll cnt = 1;

        while (x % 2 == 0) {
            x /= 2;
            cnt *= 2;
        }

        return cnt;
    };

    ll l, r;
    cin >> l >> r;

    ll tmp = l;

    vector<pair<ll, ll>> ans;

    while (tmp != r) {
        ll a = f(tmp);
        tmp += a;

        if (tmp > r) {
            ll p1 = tmp - a;
            while (tmp > r) {
                tmp -= a;
                a /= 2;
                tmp += a;
            }
            ans.push_back({p1, tmp});

        } else {
            ans.push_back({tmp - a, tmp});
        }
    }

    cout << ans.size() << endl;

    for (auto x : ans) {
        cout << x.ft << " " << x.sd << endl;
    }
}