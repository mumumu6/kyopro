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

    // 初期状態で "ABC" がいくつ存在するかを数える
    for (ll i = 0; i < n - 2; i++) {
        if (s.substr(i, 3) == "ABC") {
            cnt++;
        }
    }

    rep(i, q) {
        ll x;
        char c;
        cin >> x >> c;
        x--; // 0-indexed に変換

        // 変更がある前後の位置で "ABC" の出現を確認し、カウントを減らす
        for (ll j = max(0LL, x - 2); j <= min(n - 3, x); j++) {
            if (s.substr(j, 3) == "ABC") {
                cnt--;
            }
        }

        // 文字を変更
        s[x] = c;

        // 変更があった後で、再度 "ABC" の出現を確認してカウントを増やす
        for (ll j = max(0LL, x - 2); j <= min(n - 3, x); j++) {
            if (s.substr(j, 3) == "ABC") {
                cnt++;
            }
        }

        cout << cnt << endl;
    }
}