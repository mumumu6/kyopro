#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N, M, L;
    cin >> N >> M >> L;            // N=36, M=12, L=1e6
    vector<pair<ll, string>> v(N); // {P,S}
    rep(i, N) cin >> v[i].second >> v[i].first;
    sort(v.begin(), v.end(), greater<>());

    /* 1. 文字割当 t */
    string t = v[0].second;
    set<char> st(t.begin(), t.end());
    for (char c = 'a'; c <= 'f' && t.size() < M; ++c)
        if (!st.count(c)) t.push_back(c);
    while (t.size() < M) t.push_back('a');

    /* 2. 文字→状態 */
    vector<int> pos(6, -1);
    rep(i, M) if (pos[t[i] - 'a'] == -1) pos[t[i] - 'a'] = i;

    /* 3. 遷移行列 (double) */
    vector<vector<double>> A(M, vector<double>(M, 0.0));
    rep(i, M) A[i][(i + 1) % M] = 40.0; // 主サイクル 40 %

    auto add_full_path = [&](const string &s, double head) {
        if (s.size() < 2) return;
        int f = pos[s[0] - 'a'], to = pos[s[1] - 'a'];
        if (f < 0 || to < 0) return;
        A[f][to] += head;                           // 先頭に head %
        for (size_t i = 1; i + 1 < s.size(); ++i) { // 残りは 100 %
            int a = pos[s[i] - 'a'], b = pos[s[i + 1] - 'a'];
            if (a < 0 || b < 0) return;
            A[a][b] = 100.0; // 確率を100%に設定
        }
    };
    add_full_path(v[1].second, 30.0); // 2 位
    add_full_path(v[2].second, 30.0); // 3 位

    /* 4. 行正規化 */
    rep(i, M) {
        double s = 0;
        rep(j, M) s += A[i][j];
        if (s < 1e-9) {
            A[i][i] = 100;
            continue;
        } // 行が死んでいれば自ループ
        rep(j, M) A[i][j] = A[i][j] / s * 100.0;
    }

    /* 5. 出力 */
    rep(i, M) {
        cout << t[i];
        rep(j, M) cout << ' ' << int(A[i][j] + 0.5); // 四捨五入
        cout << "\n";
    }
}
