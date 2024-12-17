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

    ll n, k;
    string s;
    cin >> n >> k >> s;

    vector<ll> p;

    bool flag = false; // true だと 1
    p.push_back(0);

    flag = (s[0] == '1') ? true : false;
    rep(i, n) {
        if (s[i] == '1') {
            if (flag) {
                p.back()++;
            } else {
                p.push_back(1);
                flag = true;
            }
        } else {
            if (flag) {
                p.push_back(1);
                flag = false;
            } else {
                p.back()++;
            }
        }
    }

    flag = (s[0] == '1') ? true : false;

    if(flag){
        p[2*k - 4] +=  p[2*k - 2];
        p[2*k - 2] = 0;
    }else{
        p[2*k - 3] += p[2*k - 1];
        p[2*k - 1] = 0;
    }
    

    string ans = "";

    for (auto x : p) {
        if (flag) {
            ans += string(x, '1');
            flag = false;
        } else {
            ans += string(x, '0');
            flag = true;
        }
    }

    cout << ans << endl;
}