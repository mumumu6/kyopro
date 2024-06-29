#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)

int main()
{
    int x1, y1, x2, y2, x3, y3, x4, y4;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) == (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) && (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) == (x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4) && (x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4) == (x4 - x1) * (x4 - x1) + (y4 - y1) * (y4 - y1) && (x1 - x2) * (x2 - x3) + (y1 - y2) * (y2 - y3) == 0 && (x2 - x3) * (x3 - x4) + (y2 - y3) * (y3 - y4) == 0)
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}