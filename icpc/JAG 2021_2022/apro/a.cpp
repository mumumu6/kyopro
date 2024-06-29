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
#define all(x) std::begin(x), std::end(x)

int main() {
    ofstream outputfile("ans.txt");
    int n;
    string s, medal;
    while (cin >> n && n) {
        int ans1, ans2;
        int maxgold = 0, allmedal = 0;
        vector<vector<int>> a(2023, vector<int>(3, 0));

        rep(i, n) {
            int year, ss;
            cin >> year >> s >> medal;

            if (medal == "Gold") ss = 0;
            if (medal == "Silver") ss = 1;
            if (medal == "Bronze") ss = 2;
            a[year][ss]++;
        }

        reps(i, 1896, 2022) {
            if (maxgold < a[i][0]) {
                maxgold = a[i][0];
                ans1    = i;
            }
            if (allmedal < a[i][0] + a[i][1] + a[i][2]) {
                allmedal = a[i][0] + a[i][1] + a[i][2];
                ans2     = i;
            }
        }
        outputfile << ans1 << " " << ans2 << endl;
    }
}