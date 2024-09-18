#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a,b) make_pair(a,b)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(),z.end())
#define sor(z) sort(z.rbegin(),z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

ll c2(ll n) { return n * (n - 1) / 2; }


int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n;
    cin >> n;
    vec a(n);
    rep(i,n)cin >> a[i] , a[i]--;

    vecc is(n);

    rep(i,n) is[a[i]].pb(i);

    ll ans = 0;

    rep(x,n){
        ll sx = 0;
        is[x].pb(n);
        ll pre = -1;
        for (auto i : is[x]){
            ll len = i - pre - 1;
            sx += c2(len + 1);
            pre = i;
        }

        ans += c2(n+1) - sx;
    }

    cout << ans << endl;
}