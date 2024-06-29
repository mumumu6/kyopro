#include <bits/stdc++.h>
using namespace std;
int main()
{
    long long n, ans = 0;
    cin >> n;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        ans += a[i];
    }
    ans *= n - 1;
    sort(a.begin(), a.end());
    int r = n - 1;
    for (int i = 0; i < n - 1; i++)
    {
        r = max(r, i + 1);
        while (r > i && a[i + 1] + a[r + 1] >= 100000000)
            r--;
        ans -= ((n - 1) - r) * 100000000;
    }
    cout << ans << endl;
}