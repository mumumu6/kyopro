#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main() {
    int n;
    cin >> n;
    int sum = 0;
    vector<string> s(n);
    vector<int> c(n);
    rep(i, n) {
        cin >> s[i] >> c[i];
        sum += c[i];
    }

    int left = 0;
    int i = 0;

    int right = 0;

    while (left < n) {
        while (right < n - 1) {
            while (s[left][i] == s[right][i]) {
                if (i >= s[left].size()) {
                    right++;
                } else if (i >= s[right].size()) {
                    swap(s[left], s[right]);
                    right++;
                }
                i++;
            }
            if (s[left][i] > s[right][i]) {
                right++;
            } else if (s[left][i] < s[right][i]) {
                swap(s[left], s[right]);
            }
        }
        left++;
        right = left + 1;
    }

    while (sum >= n) {
        sum -= n;
    }
    cout << s[sum] << endl;
}