#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'
#define debug(x) cerr << #x << " = " << x << el

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k;
    cin >> n >> k;

    string s;
    string t(n, '.');
    cin >> s;

    string s1 = s;
    reverse(all(s1));

    vector<ll> cnt1(n + 1, 0);
    vector<ll> cnt1m(n + 1, 0);
    vector<ll> cnt2(n + 1, 0);
    vector<ll> cnt2m(n + 1, 0);

    bool iso  = false;
    bool iso2 = false;

    rep(i, n) {
        if (s[i] == 'o') {
            cnt1[i + 1]  = cnt1[i] + 1;
            cnt1m[i + 1] = cnt1m[i] + 1;
            iso          = true;
        }
        if (s[i] == '.') {
            cnt1[i + 1]  = cnt1[i];
            cnt1m[i + 1] = cnt1m[i];
            iso          = false;
        }
        if (s[i] == '?') {
            cnt1m[i + 1] = cnt1m[i];

            if (iso || (i + 1 < n && s[i + 1] == 'o')) {
                cnt1[i + 1] = cnt1[i];
                iso         = false;
            } else {
                cnt1[i + 1] = cnt1[i] + 1;
                iso         = true;
            }
        }

        if (s1[i] == 'o') {
            cnt2[i + 1]  = cnt2[i] + 1;
            cnt2m[i + 1] = cnt2m[i] + 1;
            iso2         = true;
        }
        if (s1[i] == '.') {
            cnt2[i + 1]  = cnt2[i];
            cnt2m[i + 1] = cnt2m[i];
            iso2         = false;
        }
        if (s1[i] == '?') {
            cnt2m[i + 1] = cnt2m[i];

            if (iso2 || (i + 1 < n && s1[i + 1] == 'o')) {
                cnt2[i + 1] = cnt2[i];
                iso2        = false;
            } else {
                cnt2[i + 1] = cnt2[i] + 1;
                iso2        = true;
            }
        }
    }

    // for (auto &i : cnt1) { debug(i); }
    // cout << el;
    // for (auto &i : cnt2) { debug(i); }
    // cout << el;

    rep(i, n) {
        if (s[i] == 'o') t[i] = 'o';
        if (s[i] == '.') t[i] = '.';

        if (s[i] == '?') {
            if (i - 1 >= 0 && s[i - 1] == 'o') {
                t[i] = '.';
                continue;
            } else if (i + 1 < n && s[i + 1] == 'o') {
                t[i] = '.';
                continue;
            }

            ll var;
            ll varm;
            ll var2m = cnt1m[i] + cnt2m[n - 1 - i];
            ll var2  = cnt1[i] + cnt2[n - 1 - i];

            if (i - 1 >= 0 && n - 2 - i >= 0) {

                var  = cnt1[i - 1] + cnt2[n - 2 - i];
                varm = cnt1m[i - 1] + cnt2m[n - 2 - i];

            } else if (i - 1 == -1) {
                var  = cnt2[n - 2 - i];
                varm = cnt2m[n - 2 - i];
            } else if (n - 2 - i == -1) {
                var  = cnt1[i - 1];
                varm = cnt1m[i - 1];
            }

            if (var >= k - 1 && varm <= k - 1 && var2 >= k) {
                t[i] = '?';
                // debug(var);
                // debug(var2);
                // debug(cnt1[i]);
                // debug(cnt2[n - 1 - i]);

                // cout << "---------------" << el;
            } else if (var2 < k) t[i] = 'o';
        }
    }

    cout << t << el;
    return 0;
}