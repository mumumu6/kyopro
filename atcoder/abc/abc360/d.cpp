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
    ll n, t;
    string s;
    cin >> n >> t >> s; // 左に進むありがどうなるかを考える,0だと左
    ll ans = 0;
    vector<pair<ll, char>> x(n);
    rep(i, n) {
        cin >> x[i].ft;
        x[i].sd = s[i];
    }
    sort(all(x));

    vector<ll> r; // 左に進むありの左に右行くありが何匹いるか持つ配列
    ll rnum = 0; // 今右に進むありが何匹いるか

    vector<tuple<ll, char, ll>> after(n);
    rep(i, n) {
        if (x[i].sd == '0') {
            r.push_back(rnum);
            after[i] = make_tuple(x[i].ft - t, x[i].sd, rnum);

        } else {
            rnum++;
            after[i] = make_tuple(x[i].ft + t, x[i].sd, rnum);
        }
    }
    sort(all(after));
    ll newr = 0;
    rep(i,n){
        if(get<1>(after[i]) == '0'){
            ans += get<2>(after[i]) - newr;
        }else{
            newr++;
        }
    }
    cout << ans << endl;
}