#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;
using namespace atcoder;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    ofstream outputfile("output.txt");

    ll n, m;
    cin >> n >> m;

    vector<vector<ll>> g(n);
    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].push_back(b);
    }

    // グラフの密度を計算
    double density = double(m) / (n * (n - 1) / 2.0);

    // 強連結成分（SCC）を計算
    scc_graph scc(n);
    for (ll i = 0; i < n; i++) {
        for (ll j : g[i]) {
            scc.add_edge(i, j);
        }
    }
    auto sccs = scc.scc();
    vector<ll> scc_size(sccs.size());
    for (ll i = 0; i < sccs.size(); i++) {
        scc_size[i] = sccs[i].size();
    }

    // 強連結成分のノード数の頻度を集計
    map<ll, ll> size_frequency;
    for (ll size : scc_size) {
        if (size > 0) { // ノード数が0のものはスキップ
            size_frequency[size]++;
        }
    }

    // グラフが連結かどうかを確認
    vector<bool> visited(n, false);
    function<void(ll)> dfs = [&](ll v) {
        visited[v] = true;
        for (ll u : g[v]) {
            if (!visited[u]) dfs(u);
        }
    };
    dfs(0);
    bool is_connected =
        all_of(visited.begin(), visited.end(), [](bool v) { return v; });

    // 結果を出力
    outputfile << "ノード数: " << n << endl;
    outputfile << "エッジ数: " << m << endl;
    outputfile << "グラフの密度: " << density << endl;

    outputfile << "強連結成分数: " << sccs.size() << endl;
    outputfile << "強連結成分のノード数の頻度:" << endl;
    ll ans = 0;
    for (const auto &[size, frequency] : size_frequency) {
        outputfile << size << "の強連結成分が " << frequency << " 個" << endl;
        ans += frequency * (size * (size - 1) / 2);
    }
    outputfile << "ans = " << ans << endl;
    outputfile << "グラフが連結かどうか: " << (is_connected ? "はい" : "いいえ")
               << endl;

    return 0;
}
