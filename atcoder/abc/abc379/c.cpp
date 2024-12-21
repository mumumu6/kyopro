#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m;
    cin >> n >> m;
  
    ll now = -1;
    vector<pll> a(m);
    rep(i, m) {
        cin >> a[i].ft;
        a[i].ft--;
    }
    rep(i, m) cin >> a[i].sd;

    bool flag = false;
    so(a);

    ll ans    = 0;

  
    
    rep(i, m) {
        if(now  + 1 <  a[i].ft ){
            cout << -1 << endl;
            return 0;
        }

        ans += (now + 1 - a[i].ft + now - a[i].ft  + a[i].sd) * (a[i].sd) / 2;

        now += a[i].sd;
        
        // cout << "now: " << now << " ans: " << ans << " i: " << i << endl;
    }


    if(now != n -1){
        cout << -1 << endl;
        return 0;
    }

    cout << ans << endl;
}