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

    ll h, w, m;
    cin >> h >> w >> m;

    vector<tuple<ll, ll, ll>> p(m);
    rep(i, m) {
        ll t, a, x;
        cin >> t >> a >> x;
        a--;

        p[i] = make_tuple(t, a, x);
    }

    reverse(all(p));

    map<ll, ll> mp;
    vector<bool> used1(h, false);
    vector<bool> used2(w, false);
    ll sum1 = 0;
    ll sum2 = 0;

    rep(i, m) {
        auto [t, a, x] = p[i];

        if (t == 1) {
            if (used1[a]) continue;
            if(w - sum2 == 0) continue;
            mp[x] += w - sum2;

            sum1++;
            used1[a] = true;
        } else {
            if (used2[a]) continue;
            if(h - sum1 == 0) continue;

            mp[x] += h - sum1;

            sum2++;
            used2[a] = true;
        }
    }

    rep(i,h){
        if(used1[i]) continue;
        if(w - sum2 == 0) continue;

        mp[0] += w - sum2;
    }
    cout << mp.size() << endl;
    for (auto [k, v] : mp) {
        cout << k << " " << v << endl;
    }
}