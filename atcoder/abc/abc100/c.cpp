#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n;
    int ans=0;
    cin >> n;
    vector<int> a(n);
    for (auto &c : a) cin >> c;
    rep(i, n){
        while(a[i] % 2 == 0){
            a[i] /= 2;
            ans++;
        }
    }
    cout << ans << endl;
}