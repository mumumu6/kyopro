#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int n;
    cin >> n;
    vector<pair<pair<ll, ll>, int>> a(n);
    rep(i, n) {
        cin >> a[i].first.first >> a[i].first.second;
        a[i].second = i + 1;
    }
    vector<int> ans;
    sort(a.begin(),a.end(),greater<pair<pair<ll, ll>, int>>());
    int check = a[0].first.second;
    rep(i, n){
        if(check >= a[i].first.second){
            ans.push_back(a[i].second);
            check = a[i].first.second;
        }
    }
    sort(ans.begin(), ans.end());
    int m = ans.size();
    cout << m << endl;
    rep(i, m-1){
        cout << ans[i] << " ";
    }
    cout << ans[m-1] << endl;
}