#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a,b) make_pair(a,b)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(),z.end())
#define sor(z) sort(z.rbegin(),z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    ofstream outputfile("test.txt");

    ll n, m;
    cin >> n >> m;
    vecc g(n), gg(n);
    vector<bool> used(n, false);
    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].pb(b);
        gg[b].pb(a);
    }

    stack<ll> s, back;
    int k = 0;
    vector<bool> done(n, false);

    // 第一段階のDFSでノードをバックスタックに格納
    rep(i, n) {
        if (used[i]) continue;
        s.push(~i);
        s.push(i);
        outputfile << "Start DFS for node " << i << "\n";
        while (!s.empty()) {
            ll v = s.top();
            s.pop();

            if (v >= 0) {
                used[v] = true;
                for (auto u : g[v]) {
                    if (!used[u]) {
                        outputfile << "Visit " << u << " from " << v << "\n";
                        s.push(~u);
                        s.push(u);
                    }
                }
            } else {
                back.push(~v);
                outputfile << "Backtrack " << ~v << "\n";
            }
        }
    }

    // usedフラグをリセット
    fill(all(used), false);
    ll ans = 0;
    ll cnt = 0;

    // 第二段階のDFSで強連結成分のサイズを数える
    function<void(ll)> dfs2 = [&](ll pos) {
        used[pos] = true;
        cnt++;
        for (int i : gg[pos]) {
            if (!used[i]) dfs2(i);
        }
    };

    multiset<ll> scc_sizes;

    while (!back.empty()) {
        ll v = back.top();
        back.pop();
        cnt = 0;
        if (!used[v]) {
            dfs2(v);
            scc_sizes.insert(cnt);
            ans += cnt * (cnt - 1LL) / 2LL;
            outputfile << "SCC of size " << cnt << "\n";
        }
    }

    // 強連結成分のサイズの頻度を出力
    rep(i, 1000) {
        if (scc_sizes.count(i) > 0) {
            outputfile << i << " : " << scc_sizes.count(i) << "\n";
        }
    }

    outputfile << "Total pairs in SCCs: " << ans << "\n";

    return 0;
}
