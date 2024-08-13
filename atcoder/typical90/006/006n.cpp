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
    int n, k;
    cin >> n >> k;

    string ss;
    cin >> ss;
    string s = ' ' + ss;

    vector<vector<int>> al(n + 1, vector<int>(26, -1));

    for (int i = n; i >= 0; i--) {
        rep(j,26){
            if(i == 0)continue;
            if(s[i] - 'a' == j){
                al[i-1][j] = i;
            }else{
                al[i-1][j] = al[i][j];
            }
        }
    }

    string ans = "";
    int now    = 0;
    int h      = 0;
    rep(i, k) {
        now       = 0;
        bool flag = false;

        while (!flag) {
            if (al[h][now] != -1 && al[h][now] <= n - k + i + 1) {
                h = al[h][now];
                ans += (char)('a' + now);
                flag = true;
            } else {
                now++;
            }
        }
    }
    cout << ans << endl;
}
