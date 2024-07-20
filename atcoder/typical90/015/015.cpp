#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint1000000007;
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

vector<mint> fact(101010);
vector<mint> factinv(101010);

void init() {
    fact[0]    = 1;
    factinv[0] = 1;

    rep(i, 101010 - 1) {
        fact[i + 1]    = fact[i] * (i + 1);
        factinv[i + 1] = fact[i + 1].inv();
    }
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    cin >> n;
    vector<mint> ans(n, 0);
    init();

    mint nck;
    reps(b, 1, n + 1) {
        reps(k, 1, n + 1) {
            if (n - (b - 1) * (k - 1) - k < 0) break;

            nck = fact[n - (b - 1) * (k - 1)] * factinv[k] *
                  factinv[n - (b - 1) * (k - 1) - k];

            // cout << "b " << b << " K " << k << " nck " << nck.val() << endl;
            ans[b - 1] += nck;
        }
    }

    rep(i, n) cout << ans[i].val() << endl;
}