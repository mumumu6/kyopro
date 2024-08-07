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

    ll h, w;
    cin >> h >> w;

    if(h == 1 || w == 1) {
        cout << h * w << endl;
        return 0;
    }

    if (w % 2 == 0) {
        if (h % 2 == 0)
            cout << w * h / 4 << endl;
        else
            cout << w * (h / 2 + 1) / 2 << endl;
    } else {
        if (h % 2 == 0)
            cout << (w / 2 + 1) * h / 2 << endl;
        else
            cout << (w / 2 + 1) * (h / 2 + 1)  << endl;
    }
}