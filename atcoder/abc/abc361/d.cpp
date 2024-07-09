#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    string s, t;
    cin >> n >> s >> t;

    s += "..";
    t += "..";

    map<string, int> dp;
    dp[s] = 0;
    queue<string> que;
    que.push(s);

    while (!que.empty()) {
        string str = que.front();
        que.pop();
        int j = 0;
        while (str[j] != '.') j++;

        rep(i, n + 1) {
            string ns = str;
            if (str[i] == '.' || str[i + 1] == '.') continue;

            swap(ns[i], ns[j]);
            swap(ns[i + 1], ns[j + 1]);
            if (dp.find(ns) == dp.end()) {
                que.push(ns);
                dp[ns] = dp[str] + 1;
                // cout << ns << endl;
            }
        }
    }

    if (dp.find(t) == dp.end()) {
        cout << -1 << endl;
        return 0;
    }

    cout << dp[t] << endl;
}