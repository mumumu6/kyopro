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

    string s, t;
    vector<string> x;
    cin >> s >> t;

    ll siz     = s.size();
    string tmp = s;

    if(s == t){
        cout << 0 << '\n';
        return 0;
    }

    rep(i, siz) {
        if (s[i] != t[i] && s[i] > t[i]) {
            s[i] = t[i];
            tmp  = s;
            x.pb(s);
        } else if (s[i] != t[i] && s[i] < t[i]) {
            tmp    = s;
            tmp[i] = t[i];
        } else if (i == siz - 1) {
            s = tmp;
            x.pb(s);
        }
    }

    rep(i,siz){
        if(s[siz - 1- i] != t[siz - 1 -i]){
            s[siz - 1 - i] = t[siz - 1 - i];
            x.pb(s);
        }
    }

    cout << x.size() << '\n';
    for (auto i : x) {
        cout << i << '\n';
    }
}