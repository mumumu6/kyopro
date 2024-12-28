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

    ll k;
    cin >> k;

    string s, t;
    cin >> s >> t;

    ll ns = s.size();
    ll nt = t.size(); 

    if (abs(ns - nt) > k) {
        cout << "No" << endl;
        return 0;
    }

    if (s.size() == t.size()) {
        ll dif = 0;

        rep(i, s.size()) {
            if (s[i] != t[i])  dif++; 
        }

        if(dif <= k){
            cout << "Yes" << endl;
            return 0;
        }else{
            cout << "No" << endl;
            return 0;
        }

    }

    // s.size() < t.size() にする
    if( s.size() > t.size() ){
        swap(s, t);
    }

    string st = "";
    rep(i, s.size()){
        if(s[i] == t[i]){
            st += s[i];
        }else{
            st += t[i];

            reps(j, i, s.size()){
                st += s[j];
            }

            

            break;
        }

        if(i == s.size()-1){
            st += t[s.size()];
        }
    }


    if(st == t){
        cout << "Yes" << endl;
    }else{
        cout << "No" << endl;
    }

}