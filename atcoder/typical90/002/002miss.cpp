//この解法はS＋Tも条件を満たすことを考慮できておらず不適切であやまり


#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

set<string> func(set<string> s) {
    set<string> ans;

    for (auto c : s) {
        ans.insert(c + "()");
        ans.insert("(" + c + ")");
        ans.insert("()" + c);
    }

    return ans;
}

int main() {
    int n;
    cin >> n;
    if (n % 2 == 1) {
        return 0;
    }

    set<string> ss{"()"};

    rep(i, n / 2 - 1) ss = func(ss);
   
    for(string s : ss){
        cout << s << endl;
    }
}