#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)

int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    
    rep(i,n){
        cin >> a[i];
    }
    
    int ans = 0;
    int seat = k;
    int count = 0;
    while (count < n)
    {
        if (seat >= a[count])
        {
            seat -= a[count];
            count++;
        }
        else
        {

            ans++;
            seat = k;
        }
    }
    cout << ans + 1 << endl;
}