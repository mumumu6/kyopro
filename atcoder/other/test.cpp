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

bool iskaibun(string s) {
    string ns = s;
    reverse(all(ns));
    return s == ns;
    // 回文ならtrue
}

int main() {
    int n, k;
    string s;
    cin >> n >> k >> s;
    map<string, mint> dp;
    dp[""] = 1;

    rep(i, n) {
        map<string, mint> old;
        swap(old, dp);
        for (auto [t, num] : old) {
            for (char c = 'A'; c <= 'B'; c++) {
                if (s[i] != '?' && s[i] != c) continue;
                string nt = t + c;
                if (nt.size() == k && iskaibun(nt)) continue;
                if (nt.size() == k) nt.erase(nt.begin());
                dp[nt] += num;
            }
        }
    }
    mint ans = 0;
    for (auto [t, num] : dp) ans += num;

    cout << ans.val() << endl;
    return 0;
}