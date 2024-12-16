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
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    vector<ll> score(5);
    rep(i,5){
        cin >> score[i];
    }

    vector<pair<ll, string>> s;


    rep(bit, 32) {
        if (bit == 0) continue;
        ll tmp = 0;
        string name = "";

        rep(j, 5) {
            if (bit & (1 << j)) {
                tmp += score[j];
                name += 'A' + j;
            }
        }
        s.push_back(make_pair(tmp, name));
    }

    // 前の要素について降順にソート
    sort(all(s), []( auto a,  auto b) {
        if (a.first != b.first) {
            return a.first > b.first; 
        }
        return a.second < b.second; 
    });
    

    for(auto x : s){
        cout << x.sd << endl;
    }
}