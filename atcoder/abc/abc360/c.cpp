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
    ll n;
    cin >> n;
    vector<pair<ll, ll>> a(n);
    rep(i, n) cin >> a[i].ft;
    rep(i, n) cin >> a[i].sd;

    int ans = 0;
    sort(all(a));
    int check =a[0].ft;
    reps(i,1,n){
        while(a[i].ft  == check){
            ans += a[i - 1].sd;
            i++;
        }
        check = a[i].ft;
    }

    cout << ans << endl;
}