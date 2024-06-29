#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)

int main()
{
    int d, x, y;
    cin >> d >> x >> y;
    if (-d <= x + y && x + y <= d && -d <= x - y && x - y <= d)
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}