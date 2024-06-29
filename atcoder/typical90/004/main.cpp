#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int h, w;
    cin >> h >> w;
    vector<vector<int>> a(h, vector<int>(w));
    rep(i, h) rep(j, w) cin >> a[i][j];

    vector<int> row(h,0), col(w,0);
    rep(i, h) rep(j, w) row[i] += a[i][j];
    rep(i,w)rep(j,h) col[i] += a[j][i];

    rep(i,h)rep(j,w){
        if(j == w-1){
            cout << row[i] + col[j] -a[i][j] << endl;
            continue;
        }

        
        cout << row[i] + col[j] -a[i][j] << " ";
        
    }
}