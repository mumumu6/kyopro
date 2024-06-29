#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main() {
    int h;
    cin >> h;

    int hight = 0;
    int day = 0;
    while (h - hight >= 0) {
        hight += hight + 1;
        day++;
      
    }
    cout << day << endl; 
}