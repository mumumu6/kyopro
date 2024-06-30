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
    cout << fixed << setprecision(16);
    ll n, k;
    cin >> n >> k;
      
    mint d = 1;
    rep(i,k){
        d = mint(n*n -2 * n) * mint(n*n).inv() * d + mint(2)* mint(n*n).inv();
    }
    mint ans = d;
     d = 0;
    rep(i,k){
        d = mint(n*n -2 * n)/mint(n*n) * d + mint(2) * mint(n*n).inv();
    }
    ans += d * mint((n+2)*(n-1)) * mint(2).inv();
    cout << ans.val() << endl;

}