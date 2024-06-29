#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)

void hoge(function<string(string)> f) {
    string s;
    cin >> s;
    s = f(s);
    cout << s << endl;
}

int main() {
    int a = 235135235;
    hoge([&](string s) -> string {
        a = 0;
        return "Hello, " + s + "!" + to_string(a);
    });
    cout << a << endl;
}