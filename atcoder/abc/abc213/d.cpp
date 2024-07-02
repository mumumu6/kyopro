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

vector<vector<int>> G;
vector<int> ans;

void dfs(int crr, int pre) {
    ans.push_back(crr);
    for (int nxt : G[crr])
        if (nxt != pre) {
            dfs(nxt, crr);
            ans.push_back(crr);
        }
}

int main() {
    int n;
    cin >> n;
    G.resize(n + 1);
    rep(i, n - 1) {
        int a, b;
        cin >> a >> b;
        G[a].push_back(b);
        G[b].push_back(a);
    }
    for (int i = 1; i <= n; i++) sort(G[i].begin(), G[i].end());
    dfs(1, -1);

    rep(i, ans.size()) {
        if (i > 0) cout << ' ';
        cout << ans[i];
    }
    cout << endl;
}