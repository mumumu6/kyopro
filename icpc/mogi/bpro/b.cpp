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
     ifstream inputfile("B");
     ofstream outputfile("ans.txt");
    int n;
    while (inputfile >> n && n) {
        
        // ofstream outputfile("ans.txt");
        vector<string> s(n);
        vector<char> c(n);
        int ca     = 0;
        int ansize = 0;
        rep(i, n) {
            inputfile >> s[i];
            inputfile >> c[i];
            if (s[i] == "INSERT") {
                ansize++;
            }
        }
        int k = 0;
        vector<char> ans(ansize + 1, '0');
        rep(j, n) {
            if (s[j] == "INSERT") {
                for (int i = ansize; i >= ca + 1; i--) {
                    ans[i] = ans[i - 1];
                }
                ans[ca] = c[j];
                ca++;
                //  rep(i, ansize - 1) cout << ans[i];
                //  cout << ans[ansize - 1] << " " << ca <<  endl;
            }
            if (s[j] == "LEFT" && ca != 0) ca--;

            if (s[j] == "RIGHT" && ans[ca] != '0') ca++;
        }
        rep(i, ansize - 1) outputfile << ans[i];
        outputfile << ans[ansize - 1] << endl;
    }
}