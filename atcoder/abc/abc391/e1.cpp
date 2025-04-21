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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n;
    cin >> n;

    vector<string> a(n + 1);
    ll t = 1;
    rep(i, n) t *= 3;
    ll k = t;

    cin >> a[n];

    // 各レベルの文字列を初期化
    rep(i, n) {
        k /= 3;
        a[n - 1 - i].resize(k, '0');
    }

    k = t;

    // A^(k)を計算
    rep(i, n) {
        ll cnt = 0;

        rep(j, k / 3) {
            cnt = 0;

            rep(l, 3) {
                if (a[n - i][j * 3 + l] == '1') cnt++;
            }

            if (cnt <= 1) a[n - 1 - i][j] = '0';
            else a[n - 1 - i][j] = '1';
        }

        k /= 3;

        // for (auto c : a[n - 1 - i]) { cout << c << " "; }
        // cout << endl;
    }

    k = t;

    // dpの初期化: k=0のとき、f(0,i)=1（i=1,…,3^N）
    vector<vector<ll>> dp(n + 1, vector<ll>(t + 1, 0));
    rep(i, k) {
        dp[n][i] = 1; // f(0,i)=1
    }

    rep(i, n) {

        rep(j, k / 3) {
            ll a1, a2, a3;
            a1 = a[n - i][j * 3];
            a2 = a[n - i][j * 3 + 1];
            a3 = a[n - i][j * 3 + 2];

            if (a1 == a2 && a2 == a3) {
                // 同じ値が3個あるとき
                // A_i^(k)の値を反転させるには、3つのうち少なくとも2つを反転させる必要がある
                dp[n - 1 - i][j] = min({
                    dp[n - i][j * 3] + dp[n - i][j * 3 + 1],
                    dp[n - i][j * 3] + dp[n - i][j * 3 + 2],
                    dp[n - i][j * 3 + 1] + dp[n - i][j * 3 + 2],
                });
            } else {
                // 同じ値が2個あるとき
                if (a1 == a2) {
                    // a1とa2が同じ値の場合（これが過半数を占める値）
                    // majority_valueを反転させるには、a1かa2のどちらかを反転させればよい
                    dp[n - 1 - i][j] = min(dp[n - i][j * 3], dp[n - i][j * 3 + 1]);
                } else if (a1 == a3) {
                    // a1とa3が同じ値の場合（これが過半数を占める値）
                    // majority_valueを反転させるには、a1かa3のどちらかを反転させればよい
                    dp[n - 1 - i][j] = min(dp[n - i][j * 3], dp[n - i][j * 3 + 2]);
                } else if (a2 == a3) {
                    // a2とa3が同じ値の場合（これが過半数を占める値）
                    // majority_valueを反転させるには、a2かa3のどちらかを反転させればよい
                    dp[n - 1 - i][j] = min(dp[n - i][j * 3 + 1], dp[n - i][j * 3 + 2]);
                }
            }
        }
        k /= 3;
    }

    // 最終的な答え（f(N,1)）を出力
    cout << dp[0][0] << endl;

    return 0;
}