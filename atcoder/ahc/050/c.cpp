#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

// template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
//    return os << m.val();
// }

template <class T> ostream &operator<<(ostream &os, const vector<T> &v) {
    os << '[';
    for (ll i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    return os << ']';
}
template <class T> ostream &operator<<(ostream &os, const set<T> &s) {
    os << '{';
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it != s.begin()) os << ", ";
        os << *it;
    }
    return os << '}';
}

// ----------------- デバッグ用 -----------------
namespace dbg {
template <typename... Ts> void impl(const char *names, Ts &&...xs) {
    string_view sv{names};
    array<string_view, sizeof...(Ts)> labels{};
    ll pos = 0, idx = 0;
    for (ll i = 0; i <= sv.size(); ++i) {
        if (i == sv.size() || sv[i] == ',') {
            ll start = pos;
            while (start < i && sv[start] == ' ') ++start; // 前方空白除去
            labels[idx++] = sv.substr(start, i - start);
            pos           = i + 1;
        }
    }
    ll id    = 0;
    auto out = [&](auto &&v) {
        cerr << labels[id++] << " = " << v;
        if (id < sizeof...(Ts)) cerr << ",";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int n, m;
    cin >> n >> m;

    vector<vector<char>> s(n, vector<char>(n));
    vector<int> q(n * n, 1.0);
    rep(i, n) {
        rep(j, n) {
            cin >> s[i][j];
            if (s[i][j] == '#') q[i * n + j] = 0; // 岩の位置にはロボットはいないので0で
        }
    }

    vector<ll> p;
    vector<int> nxt[4]; // 上・下・左・右の次の位置
    rep(d, 4) nxt[d].assign(n * n, 0);

    auto patch_nxt = [&](int y, int x) {
        /* --------- 列 (上・下) --------- */
        int last = -1; // 上方向に一番近い壁
        for (int i = 0; i < n; i++) {
            int id = i * n + x;
            if (s[i][x] == '#') {
                last = i;
                continue;
            }
            nxt[0][id] = (last + 1) * n + x; // ↑ で止まる場所
        }
        last = n; // 下方向
        for (int i = n - 1; i >= 0; --i) {
            int id = i * n + x;
            if (s[i][x] == '#') {
                last = i;
                continue;
            }
            nxt[1][id] = (last - 1) * n + x;
        }

        /* --------- 行 (左・右) --------- */
        last = -1;
        for (int j = 0; j < n; j++) {
            int id = y * n + j;
            if (s[y][j] == '#') {
                last = j;
                continue;
            }
            nxt[2][id] = y * n + (last + 1);
        }
        last = n;
        for (int j = n - 1; j >= 0; --j) {
            int id = y * n + j;
            if (s[y][j] == '#') {
                last = j;
                continue;
            }
            nxt[3][id] = y * n + (last - 1);
        }
    };

    // どこにいる確率が高いかの分布
    auto build_nxt = [&]() {
        // 列 ↑↓
        rep(x, n) {
            int last = -1;
            rep(y, n) {
                int id = y * n + x;
                if (s[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[0][id] = (last + 1) * n + x;
            }
            last = n;
            for (int y = n - 1; y >= 0; --y) {
                int id = y * n + x;
                if (s[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[1][id] = (last - 1) * n + x;
            }
        }
        // 行 ←→
        rep(y, n) {
            int last = -1;
            rep(x, n) {
                int id = y * n + x;
                if (s[y][x] == '#') {
                    last = x;
                    continue;
                }
                nxt[2][id] = y * n + (last + 1);
            }
            last = n;
            for (int x = n - 1; x >= 0; --x) {
                int id = y * n + x;
                if (s[y][x] == '#') {
                    last = x;
                    continue;
                }
                nxt[3][id] = y * n + (last - 1);
            }
        }
    };
    build_nxt();

    /* ---------- ③ 分布を 1 手進める関数 (整数・縮小法) ---------- */
    vector<int> buf(n * n, 0);
    auto step_prob = [&]() {
        fill(buf.begin(), buf.end(), 0);
        rep(id, n * n) {
            if (s[id / n][id % n] == '#') continue;
            int w = q[id];
            if (w == 0) continue;
            int share = (w + 3) >> 2; // (w+3)/4 切り捨てなし
            rep(d, 4) buf[nxt[d][id]] += share;
        }
        q.swap(buf);
    };

    /* ---------- ④ メインループ ---------- */
    int empty_cnt = n * n - m;
    rep(turn, empty_cnt) {
        /* A. 最小重みセル best を探す */
        int best  = -1;
        int bestw = INT_MAX;
        rep(id, n * n) {
            if (s[id / n][id % n] == '#') continue;
            if (q[id] < bestw) {
                bestw = q[id];
                best  = id;
            }
        }
        int by = best / n, bx = best % n;

        /* B. 座標を出力 */
        cout << by << " " << bx << el;

        /* C. 岩を置き、nxt と分布を更新 */
        s[by][bx] = '#';
        q[best]   = 0;
        patch_nxt(by, bx); // 行・列だけ O(N) 更新
        step_prob();       // 全体 O(N^2) 更新
    }
}