#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    int h, w;
    cin >> h >> w;
    cout << h * (w - 1) + w * (h - 1) << endl;
}