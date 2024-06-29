#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

bool judge(string s) {
    int dep = 0;
    rep(i, s.size()) {
        if (s[i] == '(') dep++;
        if (s[i] == ')') dep--;
        if (dep < 0) return false;
    }
    if (dep == 0) return true;
    return false;
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < (1 << n); i++) {
        string ss = "";
        for (int j = 0; j < n; j++) {
            // メモ : (i & (1 << j)) = 0 というのは、i の j ビット目（2^j
            // の位）が 0 であるための条件。
            // 　　　頻出なので知っておくようにしましょう。
            if ((i & (1 << j)) == 0) {
                ss += "(";
            } else {
                ss += ")";
            }
        }
        if (judge(ss)) cout << ss << endl;
    }
    return 0;
}