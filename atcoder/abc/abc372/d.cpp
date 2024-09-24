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

    ll n;
    cin >> n;
    vec h(n), a;
    rep(i, n) { cin >> h[i]; }

    vec cnt(n, 0);

    vec ans(n, 0);

    set<ll> s;

    rep(i, n) {
        if (i == 0) {
            continue;
        }

        if (h[n - i - 1] < h[n - i]) {

            s.insert(h[n - i]);
            ans[n - i - 1] = s.size();

        } else {
            for (auto it = s.begin(); it != s.end();) {
                if (*it > h[n - i - 1]) {
                    break;
                }
                if (*it < h[n - i - 1]) {
                    it = s.erase(
                        it); // 削除後は戻り値を新しいイテレータとして使う
                }
            }

            ans[n - i - 1] = s.size();
        }
    }
    rep(i, n) {
        if (i == n - 1) {
            cout << 0 << endl;
            break;
        }
        cout << ans[i + 1] + 1 << " ";
    }

    return 0;
}
