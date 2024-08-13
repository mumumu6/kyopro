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

    ll q;
    cin >> q;

    multiset<ll> s;
    set<ll> ss;

    rep(i, q) {
        ll t;
        cin >> t;

        if (t == 1) {
            ll x;
            cin >> x;

            s.insert(x);
            ss.insert(x);
        } else if (t == 2) {
            ll x;
            cin >> x;
            auto it = s.find(x);
            s.erase(it);

            if(s.find(x) == s.end()) {
                ss.erase(x);
            }
        } else if (t == 3) {
            cout << ss.size() << endl;
         }
    }
}