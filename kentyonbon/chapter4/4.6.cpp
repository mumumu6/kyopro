#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

vector<vector<int>> memo;

bool func(int i, int w, const vector<int> &a) {
    if (i == 0) {
        if (w == 0)
            return true;
        else
            return false;
    }
    if (memo[i][w] != -1) return memo[i][w];

    // a[i - 1] を選ばない場合
    if (func(i - 1, w, a)) return memo[i][w] = 1;

    // a[i - 1] を選ぶ場合
    if (func(i - 1, w - a[i - 1], a)) return memo[i][w] = 1;

    // どちらも false の場合は false
    return memo[i][w] = 0;
}

int main() {
    // 入力
    int N, W;
    cin >> N >> W;
    vector<int> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];

    // 再帰的に解く
    memo.assign(N + 1, vector<int>(W + 1, -1));
    if (func(N, W, a))
        cout << "Yes" << endl;
    else
        cout << "No" << endl;
}
