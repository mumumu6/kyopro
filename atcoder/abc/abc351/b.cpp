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
    int ans1, ans2;
    cin >> n;
    char a[n][n];
    char b[n][n];

    rep(i, n) rep(j, n) cin >> a[i][j];
    rep(i, n) rep(j, n) cin >> b[i][j];

    rep(i,n)rep(j,n){
        if(a[i][j] != b[i][j] ){
            ans1 = i;
            ans2 = j;
        }
    }
    cout << ans1 + 1 << " " << ans2 + 1 << endl;
}