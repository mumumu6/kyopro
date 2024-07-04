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
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    int n, m;
    cin >> n >> m;

    if (m != n - 1) {
        cout << "No" << '\n';
        return 0;
    }

    vecc s(n, vector<ll>());

    int bf;
    int check = 0;

    rep(i, m) {
        int u, v;
        cin >> u >> v;
        u--; // インデックスの範囲を0から始まるように調整
        v--; // インデックスの範囲を0から始まるように調整

        s[u].pb(v);
        s[v].pb(u);
    }

    rep(i, n) {
        if (s[i].size() == 2)
            continue;
        else if (s[i].size() == 1) {
            bf = i;
            check++;
            if (check > 2) {
                cout << "No" << endl;
                return 0;
            }
        } else {
            cout << "No" << endl;
            return 0;
        }
    }

    set<int> ct;
    ct.insert(bf);

    int nt = s[bf][0];

    rep(i, n - 1) {
        ct.insert(nt);
        if (s[nt].size() == 1 && ct.size() != n) {
            cout << "No" << endl;
            return 0;
        }
        if (bf == s[nt][0]) {
            nt = s[nt][1];
        } else {
            nt = s[nt][0];
        }
        bf = nt;
    }

    if (static_cast<int>(ct.size()) == static_cast<int>(n))
        cout << "Yes" << endl;
    else
        cout << "No" << endl;
    // cout << ct.size() << endl;

    return 0;
}
