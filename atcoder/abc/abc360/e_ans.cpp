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

int main() {
    ll n;
    int k;
    cin >> n >> k;

    mint p = mint(2 * (n - 1)) / mint(n).pow(2);
    mint q = mint(2) / mint(n).pow(2);
    mint d   = 1;
    for (int i = 0; i < k; ++i) {
        d = (1 - q - p) * d + q;
    }

    mint u = mint(n + 2) / 2;
    cout << (d + u * (1 - d)).val() << endl;
}