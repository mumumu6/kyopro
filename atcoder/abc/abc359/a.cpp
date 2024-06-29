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
    cin >> n;
    vector<string> s(n);
    rep(i, n){
        cin >> s[i];
    }

    int ans = 0;
    rep(i,n){
        if(s[i] == "Takahashi")ans++;
    }

    cout << ans << endl;
}