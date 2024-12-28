#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)

int main() {
    ll n, m;
    cin >> n >> m;

    map<ll, ll> xw, xb, yw, yb;
    rep(i, m) {
        ll x, y;
        char c;
        cin >> x >> y >> c;
        x--; y--; 

        if (c == 'W') { // どっちでもいい区間が
            if (!xw.count(x)) xw[x] = n;
            if (!yw.count(y)) yw[y] = n;
            xw[x] = min(xw[x], y);
            yw[y] = min(yw[y], x);
            
            
        } else {
            if (!xb.count(x)) xb[x] = -1;
            if (!yb.count(y)) yb[y] = -1;
            xb[x] = max(xb[x], y);
            yb[y] = max(yb[y], x);

        }
    }

    for(auto[x, wx] : xw) {
        xb[x] = max(xb[x], wx);
    }

    for(auto[y, wy] : yw) {
        
        yb[y] = max(yb[y], wy);
    }

    for (auto [x, wx] : xw) {
        if (xb.count(x) && xb[x] > wx) { // 白の最小血がその黒の最大より大きかったらアウト
            cout << "No" << endl;
            return 0;
        }
    }

    for (auto [y, wy] : yw) {
        if (yb.count(y) && yb[y] > wy) {
            cout << "No" << endl;
            return 0;
        }
    }



    cout << "Yes" << endl;
    return 0;
}
