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

    int n;
    string s;
    cin >> n >> s;

    auto f = [&](char s) -> int {
        if (s == 'R') return 0;
        if (s == 'P') return 1;
        if (s == 'S') return 2;
    };

    vecc dp(n + 1, vec(3, 0));

    reps(i, 1, n + 1) {
        char t = s[i - 1];
        // R グー
        dp[i][0] = max(dp[i - 1][1], dp[i - 1][2]) + (t == 'S');
        if (t == 'P') dp[i][0] = -4e18;

        // P パー
        dp[i][1] = max(dp[i - 1][0], dp[i - 1][2]) + (t == 'R');
        if (t == 'S') dp[i][1] = -4e18;

        // S チョキ
        dp[i][2] = max(dp[i - 1][0], dp[i - 1][1]) + (t == 'P');
        if (t == 'R') dp[i][2] = -4e18;
    }

    cout << max({dp[n][0], dp[n][1], dp[n][2]}) << endl;
}