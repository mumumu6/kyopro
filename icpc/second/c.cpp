#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (int i = (a); i < (b); i++)
int n;
vector<int> a(n);
bool isOK(int index, int key) {
    if (a[index] >= key)
        return true;
    else
        return false;
}

// 汎用的な二分探索のテンプレ
int binary_search(int key) {
    int left = -1; // 「index = 0」が条件を満たすこともあるので、初期値は -1
    int right =
        (int)a.size(); // 「index =
                       // a.size()-1」が条件を満たさないこともあるので、初期値は
                       // a.size()

    /* どんな二分探索でもここの書き方を変えずにできる！ */
    while (right - left > 1) {
        int mid = left + (right - left) / 2;

        if (isOK(mid, key))
            right = mid;
        else
            left = mid;
    }

    /* left は条件を満たさない最大の値、right は条件を満たす最小の値になっている
     */
    return right;
}
int main() {
    int m;
    cin >> n;

    rep(i, n) cin >> a[i];
    cin >> m;
    vector<int> b(m), c(m);
    rep(i, m) cin >> b[i];
    rep(i, m) cin >> c[i];
    sort(a.begin(), a.end());
    vector<int> sum(n);
    sum[0] = a[0];
    reps(i, 1, n) sum[i] += sum[i - 1] + a[i];

}