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

bool isKaibun(string s) {
    string ns = s;
    reverse(all(ns));
    return ns == s;
}

void f(string s, int a){
    
}

int main() {
    ofstream outputfile("ans.txt");
    string s;
    while (cin >> s && s != "#") {
        int ans  = 0;
        int len  = s.size() - 1;
        string r = "", l = "";

        bool check = false;
        rep(i, len / 2) {
            if (s[i] == s[len - i]) continue;
            r     = "";
            l     = "";
            int j = 0;

            while (s[i + j] != s[len - i]) {
                r += s[i];
                l += s[len - i];
                j++;
            }
            
        }
        cout << ans << endl;
    }
}