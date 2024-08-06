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
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    cin >> n;

    string s, t;
    cin >> s;
    bool ok = true;
    rep(i, n - 1) {
        cin >> t;
        if (s == t && s == "sweet") {
            ok = false;
            if (i == n - 2)
                cout << "Yes" << endl;
            else
                cout << "No" << endl;

            return 0;
        }
        s = t;
    }

    if (ok) cout << "Yes" << endl;
}