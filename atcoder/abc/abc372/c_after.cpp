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

    ll n, q;
    cin >> n >> q;
    string s;
    cin >> s;

    ll cnt = 0;
    vector<bool> check(n, false);

    rep(i, n) {

        if (i < n - 2 && s[i] == 'A' && s[i + 1] == 'B' && s[i + 2] == 'C') {
            cnt++;
            check[i] = true;
        }
    }

    rep(i, q) {
        ll x;
        char c;
        cin >> x >> c;
        x--;

        if (check[x] && c == 'A') {
            cout << cnt << endl;
            continue;
        } else if (x > 0 && check[x - 1] && c == 'B') {
            cout << cnt << endl;
            continue;
        } else if (x > 1 && check[x - 2] && c == 'C') {
            cout << cnt << endl;
            continue;
        } else if (check[x] && c != 'A') {
            cnt--;
            check[x] = false;
        } else if (x > 0 && check[x - 1] && c != 'B') {
            cnt--;
            check[x - 1] = false;
        } else if (x > 1 && check[x - 2] && c != 'C') {
            cnt--;
            check[x - 2] = false;
        }

        if (c == 'C' && x > 1 && !check[x - 2] && s[x - 1] == 'B' &&
            s[x - 2] == 'A') {
            cnt++;
            check[x - 2] = true;
        }
        if (x < n - 2 && !check[x] && c == 'A' && s[x + 1] == 'B' &&
            s[x + 2] == 'C') {
            cnt++;
            check[x] = true;
        }

        if (c == 'B' && x > 0 && x < n - 1 && !check[x - 1] &&
            s[x - 1] == 'A' && s[x + 1] == 'C') {
            cnt++;
            check[x - 1] = true;
        }
        s[x] = c;

        cout << cnt << endl;
    }
}