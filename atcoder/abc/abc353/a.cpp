#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)

int main()
{
    int n;
    cin >> n;
    int ans = -1;
    vector<int> h(n);
    rep(i, n)
    {
        cin >> h[i];
    }
    rep(i, n)
    {
        if (h[0] < h[i])
        {
            ans = i + 1;
            break;
        }
    }
    cout << ans << endl;
    return 0;
}