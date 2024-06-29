#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    int n;
    int ans = 0;
    vector<int> v(n);
    cin >> n;
    rep(i, n) cin >> v[i];

    for (int i = 0; i < n-1; i += 2)
    {
        if (v[i] != v[i + 2])
        {
            ans += (n -2 - i )/2 + 1;
            break;
        }
    }
    for (int i = 1; i < n - 2; i += 2)
    {
        if (v[i] != v[i + 2])
        {
            ans += (n - i - 1) / 2;
            break;
        }
    }

    cout << ans << endl;
}