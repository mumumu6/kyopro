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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n;
    string s;
    cin >> n >> s;

    vec c(n);
    rep(i,n) cin >> c[i];

    vec sum1(n+1,0), sum2(n+1,0); // 1は01, 2は10

    rep(i,n){
        if(s[i] == '0' && i % 2 == 0){
            sum1[i + 1] = sum1[i];
            sum2[i + 1] = sum2[i] + c[i];
        }else if(s[i] == '0' && i % 2 == 1){
            sum1[i + 1] = sum1[i] + c[i];
            sum2[i + 1] = sum2[i];
        } else if (s[i] == '1' && i % 2 == 0){
            sum1[i + 1] = sum1[i] + c[i];
            sum2[i + 1] = sum2[i];
        } else {
            sum1[i + 1] = sum1[i];
            sum2[i + 1] = sum2[i] + c[i];
        }
    }

    ll ans = 4e18;

    rep(i,n - 1){
        chmin(ans, sum1[i + 1] + sum2[n] - sum2[i + 1]);
        chmin(ans, sum2[i + 1] + sum1[n] - sum1[i + 1]);
    }

    cout << ans << endl;
}