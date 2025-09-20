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
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
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

template <class K, class V, class Comp, class Alloc>
ostream &operator<<(ostream &os, const map<K, V, Comp, Alloc> &m) {
    os << '{';
    for (auto &[k, v] : m) {
        if (k != m.begin()->first) os << ", ";
        os << k << ": " << v;
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
        if (id < sizeof...(Ts)) cerr << ", ";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

// S→G が到達可能か（S/G は常に通路扱い）
bool path_exists_SG(const vector<vector<char>> &b, int si, int sj, int ti, int tj) {
    int h = b.size(), w = b[0].size();
    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;

    auto ok = [&](int x, int y) {
        if (!inside(x, y, w, h)) return false;
        if ((x == si && y == sj) || (x == ti && y == tj)) return true;
        return b[y][x] != 'T';
    };

    if (!ok(si, sj) || !ok(ti, tj)) return false;
    vis[sj][si] = 1;
    q.push({si, sj});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == ti && y == tj) return true;
        rep(d, 4) {
            int nx = x + dx[d], ny = dy[d];
            if (inside(nx, ny, w, h) && !vis[ny][nx] && ok(nx, ny)) {
                vis[ny][nx] = 1;
                q.push({nx, ny});
            }
        }
    }
    return false;
}

// 1回の「壁伸ばし」トライ：seed (sx,sy) からランダム方向に“既存壁に当たるまで”壁化候補を集め、
// S→G が切れないなら確定、切れるならロールバック。
bool try_wall_extend(vector<vector<char>> &b, int sx, int sy, int si, int sj, int ti, int tj,
                     mt19937_64 &rng) {
    int h = b.size(), w = b[0].size();
    if (!inside(sx, sy, w, h)) return false;
    if ((sx == si && sy == sj) || (sx == ti && sy == tj)) return false;
    if (b[sy][sx] == 'T') return false; // すでに壁なら不可

    array<int, 4> ord{0, 1, 2, 3};
    shuffle(ord.begin(), ord.end(), rng);

    rep(k, 4) {
        int d = ord[k];
        vector<pair<int, int>> mods;
        int x = sx, y = sy;
        while (true) {
            int nx = x + (int)dx[d];
            int ny = y + (int)dy[d];
            if (!inside(nx, ny, w, h)) break;                            // 枠に到達→そこまでOK
            if ((nx == si && ny == sj) || (nx == ti && ny == tj)) break; // S/G直前までOK
            if (b[ny][nx] == 'T') break;                                 // 既存壁に到達→そこまでOK
            mods.emplace_back(nx, ny);                                   // 通路→壁候補
            x = nx;
            y = ny; // 1歩進める
        }
        if (mods.empty()) continue;

        int committed = 0;
        for (int i = 0; i < (int)mods.size(); ++i) {
            auto [ux, uy] = mods[i];
            b[uy][ux]     = 'T'; // まず置いてみる
            if (!path_exists_SG(b, si, sj, ti, tj)) {
                b[uy][ux] = '.'; // ここで切れた → 戻して打ち止め
                break;
            }
            committed++; // セーフ → 確定
        }
        if (committed > 0) return true; // 少しでも増えたら成功
    }
    return false;
}

// 壁伸ばし本体：iters 回ランダムに試す
void wall_extend(vector<vector<char>> &b, int si, int sj, int ti, int tj, int iters = 6000,
                 uint64_t seed = 123456789ULL) {
    int h = b.size(), w = b[0].size();
    mt19937_64 rng(seed);

    // 念のため初期状態で S→G が通れることを確認
    if (!path_exists_SG(b, si, sj, ti, tj)) {
        cerr << "Warning: initial S->G path does not exist!" << el;
        return;
    }

    uniform_int_distribution<int> dxrand(0, w - 1), dyrand(0, h - 1);

    rep(it, iters) {
        int sx = dxrand(rng);
        int sy = dyrand(rng);
        // 通路から始める（既存壁からでも書けるが、通路起点のほうが“埋める”効果が高い）
        if (b[sy][sx] == 'T') continue;
        // 少しだけ「壁の近傍」を優先すると伸びやすい
        bool nearWall = false;
        rep(d, 4) {
            int nx = sx + dx[d], ny = sy + dy[d];
            if (inside(nx, ny, w, h) && b[ny][nx] == 'T') {
                nearWall = true;
                break;
            }
        }
        if (!nearWall) continue;

        (void)try_wall_extend(b, sx, sy, si, sj, ti, tj, rng);
    }
}
// ---- 追記ここまで ----

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int N;
    int ti, tj;
    cin >> N >> ti >> tj;
    int sj = 0, si = N / 2; // 入口

    vector<vector<char>> b(N, vector<char>(N));
    rep(i, N) rep(j, N) cin >> b[i][j];

    // 壁伸ばし法
    wall_extend(b, /*S*/ si, sj, /*G*/ ti, tj, /*iters*/ 8000, /*seed*/ 114514ULL);

    rep(i, N) {
        rep(j, N) cout << b[i][j];
        cout << '\n';
    }
    cout.flush();

    while (true) {
        ll x, y;
        cin >> x >> y;
        if (x == ti && y == tj) break;

        ll n;
        cin >> n;
        vec xs(n), ys(n);
        rep(i, n) cin >> xs[i] >> ys[i];
        cout << 0 << endl;
    }
}
