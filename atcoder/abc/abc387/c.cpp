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
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
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

    ll l, r;
    cin >> l >> r;
    l--;

    // l の桁数を求める
    ll l_digit = 0;
    ll l_tmp   = l;
    while (l_tmp > 0) {
        l_tmp /= 10;
        l_digit++;
    }

    // r の桁数を求める
    ll r_digit = 0;
    ll r_tmp   = r;
    while (r_tmp > 0) {
        r_tmp /= 10;
        r_digit++;
    }

    ll lt = l, rt = r;
    // 各桁をベクターに格納
    vector<ll> l_vec(l_digit), r_vec(r_digit);
    rep(i, l_digit) l_vec[i] = lt % 10, lt /= 10;
    rep(i, r_digit) r_vec[i] = rt % 10, rt /= 10;

    reverse(all(l_vec));
    reverse(all(r_vec));

    auto powi = [&](ll a, ll b) { // a^b
        ll ans = 1;
        rep(i, b) ans *= a;
        return ans;
    };


    ll ansr = 0;
    rep(i, r_digit-1) {
        if(i == 0) continue;
        rep(j, 9)  ansr += powi(j+1,i);
    }
    cout << "ansr: "<< ansr << endl;


    ll tmp = 1;
    reps(i,1,r_vec.size()) {
        ll t =min( r_vec[i] + 1, r_vec[0]);
        tmp *= t;
    }
    ansr += tmp;
    cout << "ansr: "<< ansr << endl;

    rep(i,r_vec[0]){
        if(i == 0) continue;
        ansr += powi(i,r_digit-1);
    }
    cout << "ansr: "<< ansr << endl;


    ll ansl = 0;
    rep(i, l_digit-1) {
        if(i == 0) continue;
        rep(j, 9)  {ansl += powi(j+1,i);}
    }
    cout << "ansl: "<< ansl << endl;

    rep(i,l_vec[0]){
        if(i == 0) continue;
        ansl += powi(i,l_digit-1);
    }
    cout << ansl << endl;
    tmp = 1;

    reps(i,1,l_vec.size()) {
        ll t =min( l_vec[i] + 1, l_vec[0]);
        tmp *= t;
    }
    
    ansl += tmp;
    cout << ansl << endl;

    cout << "ansr: " << ansr << " ansl: " << ansl << endl;
    cout << "l_digit: " << l_digit << " r_digit: " << r_digit << endl;

    cout << ansr - ansl << endl;

// 97 なら 36+8 = 44
// 207 なら　45 + 1 + 4 = 50

}