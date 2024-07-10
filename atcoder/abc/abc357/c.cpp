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
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    cin >> n;

    rep(i, pow(3, n)) rep(j, pow(3, n)) {
        int a    = i;
        int b    = j;
        bool che = false;

        while (a > 0 && b > 0) {
            if (a % 3 == 1 && b % 3 == 1) {
                cout << '.';
                // cout << i << ' ' << j << endl;
                a /= 3;
                b /= 3;
                che = true;
                a   = 0;
                b   = 0;
            }
            a /= 3;
            b /= 3;
        }
        if (!che) cout << '#';
        if (j == pow(3, n) - 1) cout << endl;
    }
}