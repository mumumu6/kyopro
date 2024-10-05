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

    string s;
    cin >> s;

    ll n = s.size();

    ll ans = 0;

    string ss = s;

    reverse(all(ss));
    set<char> st;

    vector<ll> num(n + 1, 0);

    vector<vector<ll>> d(26, vec());

    rep(i, n) {
        d[s[i] - 'a'].pb(i);

        if (st.count(ss[i])) {
            num[i + 1] = num[i];
            continue;
        }
        st.insert(ss[i]);
        num[i + 1] = num[i] + 1;
    }

    reverse(all(num));
    bool flag = true;
    rep(i, n) {
        if (upper_bound(all(d[s[i] - 'a']), i) == d[s[i] - 'a'].end()) {
            ans += num[i + 1];
        } else if (flag) {

            ans += num[i + 1];

            flag = false;
        }else {
            ans += num[i + 1] - 1;
        }
    }

    cout << ans << endl;
}