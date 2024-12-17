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

    string s;
    ll q;
    cin >> s >> q;
    ll n = s.size();

    vec k(q);
    rep(i, q) {
        ll k;
        cin >> k;

        ll l = k % n;

        if (l == 0) {
            l = n - 1;
        } else {
            l--;
        }


        char c    = s[l];
        bool flag = true;
        while (k > n) {
            ll t = n;
            while (t < k) { t *= 2; }
            t /= 2;
            k %= t;
            flag = !flag;
            if(k == 0) {
                k = t;
            }
        }

        if (flag) {
            cout << c << " ";
        } else {
            cout << static_cast<char>(islower(c) ? toupper(c) : tolower(c)) << " ";
        }
    }
    cout << endl;
}