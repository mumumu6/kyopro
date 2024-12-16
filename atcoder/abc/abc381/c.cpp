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
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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


    ll n;
    string s;
    cin >> n >> s;
    ll cnt    = 0;
    ll tmp    = 0;
    bool flag = false; // 直前が1ならfalse,2ならtrue
    ll ans    = 0;

    rep(i, n) {
        if (!flag) {
            if (s[i] == '1') {
                cnt++;
                tmp++;
            } else if (s[i] == '/') {
                if (cnt != 0) flag = true;
            } else {
                cnt = 0;
                tmp = 0;
            }
        } else {
            if (s[i] == '1') {
                chmax(ans, tmp - cnt);
                cnt  = 1;
                tmp  = 1;
                flag = false;
            } else if (s[i] == '/') {
                chmax(ans, tmp - cnt);
                flag = false;
                cnt  = 0;
                tmp  = 0;
            } else {
                cnt--;
                if (cnt == 0) {
                    chmax(ans, tmp);
                    tmp  = 0;
                    cnt  = 0;
                    flag = false;
                }
            }
        }
    }

    chmax(ans, tmp - cnt);

    cout << ans * 2 + 1 << endl;
}