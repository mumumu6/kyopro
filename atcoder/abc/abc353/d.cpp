#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    int n;
    ll ans = 0;
    const int M = 998244353;
    vector<int> s(n);
    vector<int> count(10);
    cin >> n;
    rep(i, n) {
        cin >> s[i];
        count[s.size()-1] ++;
        ans += ll(n-1) * ll(s[i]);
    }


}