// --- 1. 標準ヘッダ ---
#include <iostream>  // 入出力 (std::cin, std::cout, std::nl)
#include <vector>    // 動的配列 (std::vector)
#include <string>    // 文字列 (std::string)
#include <algorithm> // min, max, sort, reverse, fill など (std::min, std::max, std::sort, std::reverse, std::fill)
#include <cmath>     // 数学関数 (sqrt, pow, abs など) (std::sqrt, std::pow, std::abs)
#include <numeric>   // 累積和, 順列生成, GCD, LCM など (std::accumulate, std::iota, std::gcd, std::lcm) (C++17以降)
#include <map>       // 連想配列 (std::map)
#include <set>       // 集合 (std::set)
#include <queue>     // キュー (std::queue) (BFSなど)
#include <deque>     // 両端キュー (std::deque)
#include <iomanip>   // 出力フォーマット (std::setprecision, std::setw)
// #include <bits/stdc++.h> // 全ての標準ライブラリをまとめてインクルードする場合 (上記をまとめて代用可能だが、必要なものだけが良い習慣)

// --- 2. 名前空間 ---
using namespace std;

// --- 3. 型エイリアス ---
using ll = long long;       // long long を ll と短縮
using ld = long double;     // long double を ld と短縮
using pii = pair<int, int>; // pair<int, int> を pii と短縮
using pll = pair<ll, ll>;   // pair<ll, ll> を pll と短縮

// --- 4. 定数 ---
// 無限大の値 (様々な型に対応)
const int INF_INT = 2e9; // int 型の無限大 (約 2 * 10^9)
const ll INF_LL = 9e18;  // long long 型の無限大 (約 9 * 10^18)
// 剰余計算の法 (よく使われる値)
const int MOD = 1000000007; // 最も一般的な法 (AtCoderで最も頻繁に利用される)
// const int MOD = 998244353;   // もう一つの一般的な法
const double PI = acos(-1);                              // 円周率 (std::acosは<cmath>で定義)
const string ALPHA_LOWER = "abcdefghijklmnopqrstuvwxyz"; // 小文字アルファベット文字列
const string ALPHA_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 大文字アルファベット文字列
// グリッド問題用の方向配列 (右、上、左、下、右下、右上、左下、左上)
const ll DX[] = {1, 0, -1, 0, 1, 1, -1, -1}; // x方向の移動量
const ll DY[] = {0, 1, 0, -1, 1, -1, 1, -1}; // y方向の移動量

// --- 5. マクロ ---
// ループマクロ
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)                     // 0 から n-1 まで反復 (Increment i)
#define rep1(i, n) for (ll i = 1; i <= (ll)(n); ++i)                   // 1 から n まで反復 (Increment i)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i)               // n-1 から 0 まで反復 (Decrement i)
#define repbit(bit, n) for (ll bit = 0; bit < (1LL << (ll)(n)); ++bit) // 全ての部分集合をイテレート (ビットマスク)
// コンテナのイテレーションマクロ
#define each(x, a) for (auto &x : a)             // 参照によるイテレーション (要素の変更が可能)
#define each_const(x, a) for (const auto &x : a) // const参照によるイテレーション (要素は読み取り専用)
#define each2(a, b, c) for (auto &[a, b] : c)    // ペアやタプルを含むコンテナのイテレーション (C++17 構造化束縛)
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
template <class T>
bool chmax(T &a, const T &b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}
// aをbで最小化 (bがaより小さければaをbに更新)
template <class T>
bool chmin(T &a, const T &b)
{
    if (a > b)
    {
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
// #define DEBUG
#ifdef DEBUG
// 単一変数のデバッグ出力
#define debug(x) cerr << #x << ": " << (x) << nl
// vectorのデバッグ出力
#define debug_vec(vec)               \
    {                                \
        cerr << #vec << ": ";        \
        for (const auto &elem : vec) \
            cerr << elem << " ";     \
        cerr << nl;                  \
    }
// mapのデバッグ出力
#define debug_map(m)                                  \
    {                                                 \
        cerr << #m << ": ";                           \
        for (const auto &[key, val] : m)              \
            cerr << "(" << key << ":" << val << ") "; \
        cerr << nl;                                   \
    }
// pairのデバッグ出力 (例: debug_pair(my_pair);)
#define debug_pair(p) cerr << #p << ": (" << (p).first << ", " << (p).second << ")" << nl
// 2次元vectorのデバッグ出力 (例: debug_2d_vec(matrix);)
#define debug_2d_vec(vec2d)              \
    {                                    \
        cerr << #vec2d << ":" << nl;     \
        for (const auto &row : vec2d)    \
        {                                \
            cerr << "  ";                \
            for (const auto &elem : row) \
                cerr << elem << " ";     \
            cerr << nl;                  \
        }                                \
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
void setup_fast_io()
{
    ios_base::sync_with_stdio(false); // C形式のI/OとC++ストリームの連携を解除
    cin.tie(NULL);                    // cinとcoutの連携を解除 (cinの後にcoutのフラッシュがされない)
    cout.tie(NULL);                   // coutのフラッシュを解除 (任意だが、まれに効果あり)
}

// --- 9. よく使う構造体/クラス/グローバル変数 (任意) ---
// 例: グラフのEdge構造体。よく使うグラフ表現をここに含めることができます。
struct Edge
{
    ll to;   // 行き先の頂点
    ll cost; // 辺のコスト/重み
};
// グラフの隣接リスト表現 (Edge構造体を使用しない場合)
// using Graph = vector<vector<ll>>; // 辺の重みがない、または均一な重みのグラフ用
// グラフの隣接リスト表現 (Edge構造体を使用する場合)
// using Graph = vector<vector<Edge>>; // 重み付きグラフ用

// --- ここから問題ごとのコードを記述 ---

int main()
{
    // 高速I/O設定を呼び出す (推奨)
    setup_fast_io();

    // ここに問題の解法コードを記述します
    while (true)
    {
        int n, m;
        cin >> n >> m;

        if (n == 0 && m == 0)
            break;

        vector<string> c(n);

        rep(i, n)
        {
            string t;
            cin >> t;
            c[i] = t;
        }

        // 全行全辺探索
        vector<bool> l(n, false);

        bool bad = false;

        ll r = -1;
        // 全行探索
        rep(i, n)
        {
            if (bad)
                break;

            char b = c[i][0];
            bool flag = false;
            int cnt = 0;
            for (int j = 1; j < m; j++)
            {
                if (c[i][j] != b)
                {
                    if (flag)
                    {
                        if (r == -1)
                        {
                            r = cnt;
                            cnt = 1;
                        }
                        else
                        {
                            if (r != cnt)
                            {
                                cout << -1 << nl;
                                bad = true;
                                break;
                            }
                            cnt = 1;
                        }
                    }
                    else if (!flag)
                    {
                        cnt = 1;
                        flag = true;
                    }
                }
                else
                {
                    cnt++;
                }
                b = c[i][j];
            }
        }
        if (bad)
            continue;

        // 全列探索
        rep(i, m)
        {
            if (bad)
                break;

            char b = c[0][i];
            bool flag = false;
            int cnt = 0;
            for (int j = 1; j < n; j++)
            {
                if (c[j][i] != b)
                {
                    if (i == 0)
                        l[j] = true;

                    if (flag)
                    {
                        if (r == -1)
                        {
                            r = cnt;
                            cnt = 1;
                        }
                        else
                        {
                            if (r != cnt)
                            {
                                cout << -1 << nl;
                                bad = true;
                                break;
                            }
                            cnt = 1;
                        }
                    }
                    else if (!flag)
                    {
                        cnt = 1;
                        flag = true;
                    }
                }
                else
                {
                    cnt++;
                }
                b = c[j][i];
            }
        }
        if (bad)
            continue;

        if (r == -1)
        {
            cout << 0 << nl;
            continue;
        }

        // 横チェック
        vector<string> t = {c[0], ""};
        rep(i, m)
        {
            if (t[0][i] == '#')
            {
                t[1] += ".";
            }
            else
            {
                t[1] += "#";
            }
        }
        int in = 0;
        int cnt = 0;
        bool flag = false;
        rep(i, n)
        {
            if (l[i])
            {
                if (!flag && cnt > r)
                {
                    cout << -1 << nl;
                    bad = true;
                    break;
                }
                if (flag && cnt != r)
                {
                    cout << -1 << nl;
                    bad = true;
                    break;
                }
                flag = true;
                cnt = 0;

                if (in == 1)
                {
                    in = 0;
                }
                else
                {
                    in = 1;
                }
            }
            if (c[i] != t[in])
            {
                cout << -1 << nl;
                bad = true;
                break;
            }
            cnt++;
        }

        if (bad)
            continue;

        if (cnt > r)
        {
            cout << -1 << nl;
            continue;
        }

        cout << r << nl;
    }

    return 0;
}