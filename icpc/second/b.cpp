#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

int main() {
    long long n;
    cin >> n;
    int count = 0;
    while (n > 0) {
        n = n / 2;
        count++;
    }
    cout << count << endl;
}