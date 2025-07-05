// --- 1. 標準ヘッダ ---
#include <algorithm> // min, max, sort, reverse, fill など (std::min, std::max, std::sort, std::reverse, std::fill)
#include <bits/stdc++.h> // 全ての標準ライブラリをまとめてインクルードする場合 (上記をまとめて代用可能だが、必要なものだけが良い習慣)
#include <cmath>         // 数学関数 (sqrt, pow, abs など) (std::sqrt, std::pow, std::abs)
#include <deque>         // 両端キュー (std::deque)
#include <iomanip>       // 出力フォーマット (std::setprecision, std::setw)
#include <iostream>      // 入出力 (std::cin, std::cout, std::endl)
#include <map>           // 連想配列 (std::map)
#include <numeric> // 累積和, 順列生成, GCD, LCM など (std::accumulate, std::iota, std::gcd, std::lcm) (C++17以降)
#include <queue>  // キュー (std::queue) (BFSなど)
#include <set>    // 集合 (std::set)
#include <string> // 文字列 (std::string)
#include <vector> // 動的配列 (std::vector)

// --- 2. 名前空間 ---
using namespace std;

// --- 3. 型エイリアス ---
using ll  = long long;      // long long を ll と短縮
using ld  = long double;    // long double を ld と短縮
using pii = pair<int, int>; // pair<int, int> を pii と短縮
using pll = pair<ll, ll>;   // pair<ll, ll> を pll と短縮

// --- 4. 定数 ---
// 無限大の値 (様々な型に対応)
const int INF_INT = 2e9;  // int 型の無限大 (約 2 * 10^9)
const ll INF_LL   = 9e18; // long long 型の無限大 (約 9 * 10^18)
// 剰余計算の法 (よく使われる値)
const int MOD = 1000000007; // 最も一般的な法 (AtCoderで最も頻繁に利用される)
// const int MOD = 998244353;   // もう一つの一般的な法
const double PI          = acos(-1);                     // 円周率 (std::acosは<cmath>で定義)
const string ALPHA_LOWER = "abcdefghijklmnopqrstuvwxyz"; // 小文字アルファベット文字列
const string ALPHA_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 大文字アルファベット文字列
// グリッド問題用の方向配列 (右、上、左、下、右下、右上、左下、左上)
const ll DX[] = {1, 0, -1, 0, 1, 1, -1, -1}; // x方向の移動量
const ll DY[] = {0, 1, 0, -1, 1, -1, 1, -1}; // y方向の移動量

// --- 5. マクロ ---
// ループマクロ
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)       // 0 から n-1 まで反復 (Increment i)
#define rep1(i, n) for (ll i = 1; i <= (ll)(n); ++i)     // 1 から n まで反復 (Increment i)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i) // n-1 から 0 まで反復 (Decrement i)
#define repbit(bit, n)                                                                                  \
    for (ll bit = 0; bit < (1LL << (ll)(n)); ++bit) // 全ての部分集合をイテレート (ビットマスク)
// コンテナのイテレーションマクロ
#define each(x, a) for (auto &x : a)             // 参照によるイテレーション (要素の変更が可能)
#define each_const(x, a) for (const auto &x : a) // const参照によるイテレーション (要素は読み取り専用)
#define each2(a, b, c)                                                                                  \
    for (auto &[a, b] : c) // ペアやタプルを含むコンテナのイテレーション (C++17 構造化束縛)
// ユーティリティマクロ
#define all(v) (v).begin(), (v).end()    // コンテナ全体のイテレータ範囲 (std::begin, std::end)
#define rall(v) (v).rbegin(), (v).rend() // コンテナの逆順イテレータ範囲 (std::rbegin, std::rend)
#define sz(x) ((ll)(x).size())           // コンテナのサイズを ll 型で取得 (オーバーフロー対策)
#define nl "\n"                          // 改行文字
// 真偽値出力マクロ
#define yes_no(ans) cout << ((ans) ? "Yes" : "No") << nl; // "Yes" または "No" を出力
#define YES_NO(ans) cout << ((ans) ? "YES" : "NO") << nl; // "YES" または "NO" を出力

// --- 6. 便利関数 (テンプレート) ---
// aをbで最大化 (bがaより大きければaをbに更新)
template <class T> bool chmax(T &a, const T &b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}
// aをbで最小化 (bがaより小さければaをbに更新)
template <class T> bool chmin(T &a, const T &b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

// 配列やvectorの初期化 (例: fill_vector(vec, 0))
// std::fill を使うのでこのマクロは必須ではありませんが、簡潔に書けます。
// std::fill は <algorithm> で定義されています。
#define fill_vector(vec, val) std::fill(all(vec), val)

// --- 7. デバッグマクロ (条件付きコンパイル) ---
// デバッグ出力を有効にするには、以下の行のコメントアウトを解除してください。
// 本番提出時には必ずコメントアウトするか削除してください。
#define DEBUG
#ifdef DEBUG
// 単一変数のデバッグ出力
#define debug(x) cerr << #x << ": " << (x) << endl
// vectorのデバッグ出力
#define debug_vec(vec)                                                                                  \
    {                                                                                                   \
        cerr << #vec << ": ";                                                                           \
        for (const auto &elem : vec) cerr << elem << " ";                                               \
        cerr << endl;                                                                                   \
    }
// mapのデバッグ出力
#define debug_map(m)                                                                                    \
    {                                                                                                   \
        cerr << #m << ": ";                                                                             \
        for (const auto &[key, val] : m) cerr << "(" << key << ":" << val << ") ";                      \
        cerr << endl;                                                                                   \
    }
// pairのデバッグ出力 (例: debug_pair(my_pair);)
#define debug_pair(p) cerr << #p << ": (" << (p).first << ", " << (p).second << ")" << endl
// 2次元vectorのデバッグ出力 (例: debug_2d_vec(matrix);)
#define debug_2d_vec(vec2d)                                                                             \
    {                                                                                                   \
        cerr << #vec2d << ":" << endl;                                                                  \
        for (const auto &row : vec2d) {                                                                 \
            cerr << "  ";                                                                               \
            for (const auto &elem : row) cerr << elem << " ";                                           \
            cerr << endl;                                                                               \
        }                                                                                               \
    }
#else
// DEBUGが定義されていない場合は、デバッグマクロは何もしない
#define debug(x)
#define debug_vec(vec)
#define debug_map(m)
#define debug_pair(p)
#define debug_2d_vec(vec2d)
#endif

// --- 8. 高速I/O設定 ---
// main()関数の先頭でこの関数を呼び出すことで、入出力が高速化されます。
void setup_fast_io() {
    ios_base::sync_with_stdio(false); // C形式のI/OとC++ストリームの連携を解除
    cin.tie(NULL);                    // cinとcoutの連携を解除 (cinの後にcoutのフラッシュがされない)
    cout.tie(NULL);                   // coutのフラッシュを解除 (任意だが、まれに効果あり)
}

// --- 9. よく使う構造体/クラス/グローバル変数 (任意) ---
// 例: グラフのEdge構造体。よく使うグラフ表現をここに含めることができます。
struct Edge {
    ll to;   // 行き先の頂点
    ll cost; // 辺のコスト/重み
};
// グラフの隣接リスト表現 (Edge構造体を使用しない場合)
using Graph = vector<vector<ll>>; // 辺の重みがない、または均一な重みのグラフ用
// グラフの隣接リスト表現 (Edge構造体を使用する場合)
// using Graph = vector<vector<Edge>>; // 重み付きグラフ用

// --- ここから問題ごとのコードを記述 ---
// --- グローバル変数 ---
// 各ノードの深さ
vector<long long> depth;
vector<bool> seen;

// O(V)
void dfs_tree(const Graph &graph, long long v, long long p, long long depth_current) {
    depth[v] = depth_current;

    for (long long nv : graph[v]) {
        if (nv == p) continue;

        dfs_tree(graph, nv, v, depth_current + 1);
    }
}

void run_dfs_tree(const Graph &graph, long long n, long long start_node) {
    // --- 初期化 ---
    depth.assign(n, 0);
    seen.assign(n, false);

    dfs_tree(graph, start_node, -1, 0);
}

// bfs
vector<ll> bfs(const Graph &graph, long long n, long long start_node, ll limit, ll t) {
    vector<long long> dist(n, -1);
    vector<ll> prev(n, -1);
    queue<long long> que;

    dist[start_node] = 0;
    que.push(start_node);
    ll goal = -1;

    while (!que.empty()) {
        long long v = que.front();
        que.pop();

        for (long long nv : graph[v]) {
            if (dist[nv] == -1) {
                dist[nv] = dist[v] + 1;
                prev[nv] = v;
                if (dist[nv] + t > limit) { return vector<ll>(); } // 制限を超えた場合は空のvectorを返す
                if (seen[nv]) {
                    goal = nv; // 目標ノードを見つけた場合
                    break;
                }
                que.push(nv);
            }
        }

        if (goal != -1) { break; } // 目標ノードが見つかったらループを抜ける
    }

    vector<ll> path;

    if (goal == -1) { return path; } // 目標ノードが見つからなかった場合は空のvectorを返す

    for (ll v = prev[goal]; v != -1; v = prev[v]) { path.push_back(v); }

    return path;
}

int main() {
    // 高速I/O設定を呼び出す (推奨)
    setup_fast_io();

    // ここに問題の解法コードを記述します
    while (true) {
        ll n;
        cin >> n;
        if (n == 0) break;
        Graph graph(n);
        multimap<ll, pair<ll, ll>> mp; // 時間は重複するのでmultimapを使用
        vector<ll> p(n), e(n);
        rep(i, n - 1) {
            cin >> p[i + 1] >> e[i + 1];
            p[i + 1]--;
            graph[i + 1].push_back(p[i + 1]);
            graph[p[i + 1]].push_back(i + 1);
            mp.insert({e[i + 1], {i + 1, p[i + 1]}});
        }

        ll cur_t = 0;
        run_dfs_tree(graph, n, 1);
        seen[0] = true;
        stack<ll> st;
        vector<ll> ans;
        bool ok = true;

        each2(a, b, mp) {
            ll limit = a;
            ll u     = b.first;
            ll v     = b.second;

            if (seen[u] && seen[v]) {
                continue;
            } else if (seen[u] || seen[v]) {
                if (cur_t <= limit) {
                    cur_t++;
                    if (!seen[u]) {
                        ans.push_back(u);
                        seen[u] = true;
                    } else {
                        ans.push_back(v);
                        seen[v] = true;
                    }
                    continue;
                } else {
                    ok = false;
                    break;
                }
            } else {
                if (depth[u] < depth[v]) { swap(u, v); }

                vector<ll> path = bfs(graph, n, v, limit, cur_t);
                if (path.empty()) {
                    ok = false;
                    break;
                }

                path.push_back(u); // uをパスの終端に追加

                // debug_vec(path);
                cur_t += path.size(); // +1 for the edge

                for (ll x : path) {
                    ans.push_back(x);
                    seen[x] = true;
                }
            }
        }

        if(!ok) {
            cout << "no" << nl;
            continue;
        }
        cout << "yes" << nl;
        rep(i, ans.size()) { cout << ans[i] + 1 << " "; }
        cout << nl;
    }
    return 0;
}