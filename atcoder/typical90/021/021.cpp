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

    int n, m;
    cin >> n >> m;
    vecc g(n), gg(n);
    rep(i, m) {
        int a, b;
        cin >> a >> b;
        g[a - 1].pb(b - 1);
        gg[b - 1].pb(a - 1);
    }
    stack<int> s;
    vector<bool> used(n, false);
    stack<int> count;

    // 初回DFS: すべてのノードを訪問
    rep(i, n) {
        if (!used[i]) {
            s.push(~i);
            s.push(i);
            while (!s.empty()) {
                int v = s.top();
                s.pop();
                if (v >= 0) {
                    if (!used[v]) {
                        used[v]    = true;
                        bool check = false;
                        for (auto u : g[v]) {
                            if (!used[u]) {
                                s.push(~u);
                                s.push(u);
                                check = true;
                            }
                        }
                    }
                } else {
                    count.push(~v);
                }
            }
        }
    }

    // `used` 配列をリセット
    fill(all(used), false);
    vecc scc;

    // 二回目のDFS: 強連結成分を見つける
    while (!count.empty()) {
        int v = count.top();
        count.pop();
        if (!used[v]) {
            vector<ll> component;
            stack<int> ss;
            ss.push(v);
            while (!ss.empty()) {
                int vv = ss.top();
                ss.pop();

                if (!used[vv]) {
                    used[vv] = true;
                    component.pb(vv);
                    for (auto u : gg[vv]) {
                        if (!used[u]) ss.push(u);
                    }
                }
            }
            scc.push_back(component);
            //  for (auto x : component) cout << x << " ";
            //  cout << '\n';
        }
    }

    // 強連結成分のサイズを基に計算
    ll ans = 0;
    for (auto v : scc) {
        int aa = v.size();
        ans += aa * (aa - 1) / 2;
    }

    // 結果を出力
    cout << ans << endl;
}