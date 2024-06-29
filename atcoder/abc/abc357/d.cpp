#include <bits/stdc++.h>
#include <atcoder/modint>
using namespace atcoder;
using namespace std;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    ll n,x;
    cin >> n;
    mint r = 1;
    x=n;
    while(x){
        x/=10;
        r*=mint(10);
    }
    mint ans=mint(n)*(r.pow(n)-mint(1))*((r-mint(1)).inv());
	cout<<(ans.val())<<endl;
	return 0;
}