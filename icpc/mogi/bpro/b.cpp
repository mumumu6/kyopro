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

int main() {
    int n;
    cin >> n;
    // ofstream outputfile("ans.txt");
    vector<string> s(n);
    vector<char> c(n);
    int ca     = 0;
    int ansize = 0;
    rep(i, n) {
        cin >> s[i];
        cin >> c[i];
        if (s[i] == "INSERT") {
            ansize++;
        }
    }
    int k = 0;
    vector<char> ans(ansize + 1, '0');
    rep(j, n) {
        if (s[j] == "INSERT") {
            for (int i = ansize; i >= ca; i--) {
                ans[i] = ans[i - 1];
            }
            ans[ca] = c[j];
            ca++;
        }
        if (s[j] == "LEFT" && ca != 0) {
            ca--;
        }
        if (s[j] == "RIGHT" && ans[ca + 1] != '0') {
            ca++;
        }
    }
    rep(i, ansize) cout << ans[i]; 
}