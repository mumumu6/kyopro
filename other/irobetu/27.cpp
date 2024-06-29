#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    vector<int> a(9);
    vector<int> b(8);
    int ans = 0;
    rep(i, 9)
    {
        cin >> a[i];
        ans += a[i];
    }
    rep(i, 8)
    {
        cin >> b[i];
        ans -= b[i];
    }

    cout << ans + 1 << endl;
}