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
    string s, t;
    cin >> s >> t;
    string ans;
    int size = s.size();

    reps(w,1,size){
        vector<string> ss(w ,"");
        rep(i,size){
            ss[ i % w] += s[i];
        }
        rep(i,w){
            if(ss[i] == t){
                cout << "Yes" << endl;
                return 0;
            }
        }
    }

    cout << "No" << endl; 
}