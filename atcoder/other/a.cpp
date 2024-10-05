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

    auto g = [&](double x, double y) -> double {
        if (abs(x + 1 / y) <= 1e-9) return 2e9;
        return (x + 1 / y);
    };

    auto f = [&](double a, double b, double c, double d, double e) -> double {
        double ans = g(d, e);
        if (ans == 2e9) return 2e9;
        ans = g(c, ans);
        if (ans == 2e9) return 2e9;
        ans = g(b, ans);
        if (ans == 2e9) return 2e9;
        ans = g(a, ans);
        if (ans == 2e9) return 2e9;

        return ans;
    };

    vector<double> s = {1, 2, 3, 4, 5, -1, -2, -3, -4, -5};

    double m = 2e9;
    sort(s.begin(), s.end());

    vector<ll> alp;

    do {
        double ans = f(s[0], s[1], s[2], s[3], s[4]);

        if (chmin(m, ans)) {
            alp = {s[0], s[1], s[2], s[3], s[4]};
        };
    } while (next_permutation(all(s)));

    cout << "ans : " << m << endl;
    rep(i, 5) {
        char c = 'a' + i;
        cout << c << ": " << alp[i] << " ";
    }
    cout << endl;
}