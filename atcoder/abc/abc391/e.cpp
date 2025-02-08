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
vector<ll> sin45 = { 0, 1, 1, 1, 0, -1, -1, -1 }, cos45 = { 1, 1, 0, -1, -1, -1, 0, 1 };
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0,-1, 0};
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
    cin >> n;
    ll t = 1;
    rep(i,n){
        t *= 3;
    }

    ll r = t;

    vector<char> a(t);
    vector<vector<char>> b(n,vector<char>(n));
    rep(i,t)cin >> a[i];
   
    b[0] = a;

    rep(i,n - 1){
        rep(j, r / 3){
            ll cnt = 0;
            rep(k, 3){
                if(b[i][j * 3 + k] == '1')cnt++;
            }

            if(cnt <= 1)b[i + 1][j] = '0';
            else b[i + 1][j] = '1';
        }
        r /= 3;
    }

    reverse(all(b));

    ll orig = b[0][0];
    
    // 以下，最小反転個数を求めるdpを再帰的に実装
    const ll INF = 1e9;
    function<pair<ll,ll>(ll, ll)> dp = [&](ll l, ll len) -> pair<ll,ll> {
        if(len == 1) {
            ll bit = a[l];
            // 反転しなくても作れる場合は 0，反転するなら 1 を加算
            return { bit == 0 ? 0LL : 1LL, bit == 1 ? 0LL : 1LL };
        }
        ll part = len / 3;
        auto left  = dp(l, part);
        auto mid   = dp(l + part, part);
        auto right = dp(l + 2 * part, part);
        pair<ll,ll> res = { INF, INF };
        // 3つの部分それぞれ0,1にする選択肢（全8通り）を全探索
        for(int mask = 0; mask < 8; mask++){
            int cnt0 = 0, cnt1 = 0;
            ll cost = 0;
            int bits[3] = { (mask >> 0) & 1, (mask >> 1) & 1, (mask >> 2) & 1 };
            cost += (bits[0] == 0 ? left.first : left.second);
            cost += (bits[1] == 0 ? mid.first  : mid.second);
            cost += (bits[2] == 0 ? right.first : right.second);
            rep(i, 3){
                if(bits[i] == 0) cnt0++; else cnt1++;
            }
            // 多数決：0が2以上なら結果は0，1が2以上なら結果は1
            if(cnt0 >= 2) res.first = min(res.first, cost);
            if(cnt1 >= 2) res.second = min(res.second, cost);
        }
        return res;
    };
    
    auto dp_res = dp(0, t);
    // 反転させたいのは元の最終結果と反対の値
    ll target = (orig == 0 ? 1 : 0);
    ll ans = target == 0 ? dp_res.first : dp_res.second;
    cout << ans << "\n";
    
    return 0;

}