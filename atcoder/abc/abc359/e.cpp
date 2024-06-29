#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (int)(n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
#define ft first
#define sd second

int main() {
    
    ll N;
    cin >> N;
    // 高さについて単調減少になるように (高さ, 個数) を管理する
    vector<pair<ll, ll>> rectangles;
    // 1 + ∑ 高さ × 個数
    ll ans = 1;
    for (ll i = 0, H; i < N; ++i) {
        cin >> H;
        // 高さは H 、はじめ個数は 1
       ll count = 1;
        // H より低いものがある限り更新
        while (!empty(rectangles) && rectangles.back().ft <= H) {
            const auto &[h, c] = rectangles.back();
            // 合計から h × c を引いて
            ans -= h * c;
            // 個数を増やして
            count += c;
            // 列から取り除く
            rectangles.pop_back();
        }
        // 合計に H × count を足す
        ans += H * count;
        rectangles.emplace_back(H, count);
        cout << ans << " ";
    }
    cout << endl;
    return 0;
}