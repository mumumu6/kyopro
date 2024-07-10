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
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    ll n;
    cin >> n;
    vector<ll> s(n);
    vector<ll> t(n);
    rep(i,n)cin >> s[i] >> t[i];
    sort(s.begin(),s.end());
    sort(t.begin(),t.end());
    ll r = 0;
    ll ans = n*(n-1)/2;
    rep(i,n){
        while(r < n && s[r] <= t[i])r++;
        ans -= n-r;
    }
    cout << ans << endl;


    
    
}