#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second
int main() {
    int n, k;
    string s;
    cin >> n >> k >> s;

    int count = 0;
    int i     = 0;
    int minwh = -1;
    vector<char> min(k, 'z');
    while (i < k) {
        count = minwh + 1;
        while (count <= n - k + i) {
            if (s[count] < min[i]) {
                min[i] = s[count];
                minwh  = count;
            }
            count++;
        }
        i++;
    }

    for(auto c :min) cout << c;
}