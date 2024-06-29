#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main() {
    ll n;
    cin >> n;
    vector<int> a(n);
    vector<int> b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    sort(b.begin(), b.end());
    bool ans = true;
    rep(i, n) {
        for (int j = i; j < n; j++) {
            if (a[j] <= b[i]) {
                ans = false;
            }
        }
    }

    if (ans)

        cout << "Yes" << endl;
    else
        cout << "No" << endl;
}