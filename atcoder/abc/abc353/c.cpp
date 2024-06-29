#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    ll n;
    const ll M = 100000000;
    ll ans = 0;
    cin >> n;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];

    sort(a.begin(), a.end());
    int right = n - 1;
    rep(left, n)
    {
        while (a[left] + a[right] >= M && right >= left)
        {

            right--;
        }

        ans -= ll(n - max(right, left)- 1) * ll(M);
    }

    rep(i, n) ans += ll(n - 1) * ll(a[i]);

    cout << ans << endl;
}