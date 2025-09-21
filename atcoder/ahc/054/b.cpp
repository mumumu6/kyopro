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
bool path_exists_to_G(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int h = b.size(), w = b[0].size();
    auto inside = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };
    if (!inside(sx, sy) || !inside(gx, gy)) return false;
    if (b[sx][sy] == 'T' || b[gx][gy] == 'T') return false; // S/G は '.' 前提

    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    vis[sx][sy] = 1;
    q.push({sx, sy});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) return true;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (!inside(nx, ny) || vis[nx][ny]) continue;
            if (b[nx][ny] == 'T') continue;
            vis[nx][ny] = 1;
            q.push({nx, ny});
        }
    }
    return false;
}

// S から見て、'.'（= 'T' 以外）が全部1成分かを判定
bool all_open_connected(const vector<vector<char>> &b, int sx, int sy) {
    int h = (int)b.size(), w = (int)b[0].size();
    auto inside = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };

    if (!inside(sx, sy) || b[sx][sy] == 'T') return false;

    int total = 0;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            if (b[x][y] != 'T') ++total;

    vector<vector<char>> vis(h, vector<char>(w, 0));
    queue<pair<int, int>> q;
    vis[sx][sy] = 1;
    q.push({sx, sy});
    int reached = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        ++reached;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && !vis[nx][ny] && b[nx][ny] != 'T') {
                vis[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
    }
    return reached == total;
}

// S→G の最短距離（到達不可なら大きい値）
int shortest_path_len(const vector<vector<char>> &b, int sx, int sy, int gx, int gy) {
    int h = b.size(), w = b[0].size();
    auto inside   = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };
    const int INF = 1e9;
    if (!inside(sx, sy) || !inside(gx, gy) || b[sx][sy] == 'T' || b[gx][gy] == 'T') return INF;

    vector<vector<int>> dist(h, vector<int>(w, INF));
    queue<pair<int, int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) return dist[x][y];
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && b[nx][ny] != 'T' && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    return INF;
}
// from_x,from_y から方向 dir に向かって、確率的に曲がりながら拡張する“候補経路”を作る。
// b は読み取り専用。S/G には入らない。途中で同じマスを二度踏まない（ループ抑制）。
// パラメータ:
//   p0         : 基本の曲がり確率
//   pinc       : 直進が続くほど曲がりやすくする加算（直進長に比例）
//   pmax       : 曲がり確率の上限
//   min_straight: この歩数以上までは曲がり確率を抑える（=直線を優先）
vector<pair<int, int>> make_bent_run(const vector<vector<char>> &b, int from_x, int from_y, int dir,
                                     int sx, int sy, int gx, int gy, mt19937_64 &rng, double p0 = 0.12,
                                     double pinc = 0.05, double pmax = 0.60, int min_straight = 2,
                                     int max_steps = 100000000) {
    int h = (int)b.size(), w = (int)b[0].size();
    auto inside2 = [&](int x, int y) { return 0 <= x && x < w && 0 <= y && y < h; };

    vector<pair<int, int>> mods;
    vector<vector<char>> seen(w, vector<char>(h, 0)); // その場限りの再訪防止

    int x = from_x, y = from_y;
    int straight = 0;
    uniform_real_distribution<double> urand(0.0, 1.0);

    auto can_step = [&](int dd) -> bool {
        int nx = x + (int)dx[dd], ny = y + (int)dy[dd];
        if (!inside2(nx, ny)) return false;
        // S/G は踏まない
        if ((nx == sx && ny == sy) || (nx == gx && ny == gy)) return false;
        if (b[nx][ny] == 'T') return false;
        if (seen[nx][ny]) return false;
        return true;
    };

    while ((int)mods.size() < max_steps) {
        int forward = dir;
        int left    = (dir + 3) & 3;
        int right   = (dir + 1) & 3;

        bool okF = can_step(forward);
        bool okL = can_step(left);
        bool okR = can_step(right);

        if (!okF && !okL && !okR) break;

        bool turn = false;
        // 直進が続くほど曲がりやすくする
        double p = p0;
        if (straight >= min_straight) p = min(pmax, p0 + pinc * (straight - min_straight + 1));

        if (!okF) {
            turn = true; // 前が塞がってたら必ず曲がる（曲がれなければbreak）
        } else {
            if (urand(rng) < p) turn = true;
        }

        if (turn) {
            // 左右のどちらか（両方可ならランダム）
            vector<int> choices;
            if (okL) choices.push_back(left);
            if (okR) choices.push_back(right);
            if (!choices.empty()) {
                dir = choices[rng() % choices.size()];
            } else {
                // 曲がれない → 前へ（前もダメなら抜ける）
                if (!okF) break;
            }
        }
        // 前進（dir は forward のままか、左右に変わっている）
        int nx = x + (int)dx[dir], ny = y + (int)dy[dir];
        if (!inside2(nx, ny) || b[nx][ny] == 'T' || (nx == sx && ny == sy) || (nx == gx && ny == gy))
            break;

        mods.emplace_back(nx, ny);
        seen[nx][ny] = 1;
        // 直進カウンタ更新
        if (dir == forward) ++straight;
        else straight = 1;
        x = nx;
        y = ny;
    }
    return mods;
}

// 1回の「壁伸ばし」トライ：seed (sx,sy) からランダム方向に“既存壁に当たるまで”壁化候補を集め、
// S→G が切れないなら確定、切れるならロールバック。
bool try_wall_extend(vector<vector<char>> &b, int from_x, int from_y, int sx, int sy, int gx, int gy,
                     mt19937_64 &rng, int &curr_len) {
    int h = b.size(), w = b[0].size();
    if (!inside(from_x, from_y, w, h)) return false;
    if ((from_x == sx && from_y == sy) || (from_x == gx && from_y == gy)) return false; // ← 修正
    if (b[from_x][from_y] == 'T') return false;

    array<int, 4> ord{0, 1, 2, 3};
    shuffle(ord.begin(), ord.end(), rng);

    auto adjT = [&](int x, int y) {
        int c = 0;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny, w, h) && b[nx][ny] == 'T') ++c;
        }
        return c;
    };

    rep(k, 4) {
        int d     = ord[k];
        auto mods = make_bent_run(b, from_x, from_y, d, sx, sy, gx, gy, rng,
                                  /* p0 */ 0.12, /* pinc */ 0.05, /* pmax */ 0.60,
                                  /* min_straight */ 2);
        if (mods.empty()) continue;

        int committed = 0;
        for (auto [ux, uy] : mods) {
            // 置く前に「周囲に壁が2つ以上」なら打ち止め（無駄置き回避）
            if (adjT(ux, uy) >= 2) break;

            b[ux][uy] = 'T';                      // 仮置き
            if (!all_open_connected(b, sx, sy)) { // 全通路連結を維持
                b[ux][uy] = '.';                  // 壊れるなら戻す
                break;
            }
            int nl = shortest_path_len(b, sx, sy, gx, gy);
            if (nl < curr_len) { // 最短距離が縮むなら却下
                b[ux][uy] = '.';
                break;
            }
            ++committed; // セーフ
            curr_len = nl;
        }
        if (committed > 0) return true;
    }
    return false;
}

// 壁伸ばし本体：iters 回ランダムに試す
void wall_extend(vector<vector<char>> &b, int sx, int sy, int gx, int gy, int iters = 6000,
                 uint64_t seed = 123456789ULL) {
    int h = b.size(), w = b[0].size();
    mt19937_64 rng(seed);

    uniform_int_distribution<int> dxrand(0, w - 1), dyrand(0, h - 1);

    int curr_len = shortest_path_len(b, sx, sy, gx, gy);

    rep(it, iters) {
        int from_x = dxrand(rng);
        int from_y = dyrand(rng);
        // 通路から始める（既存壁からでも書けるが、通路起点のほうが“埋める”効果が高い）
        if (b[from_x][from_y] == 'T') continue;
        ll cnt = 0;
        // 少しだけ「壁の近傍」を優先すると伸びやすい
        bool nearWall = false;
        ll to_x, to_y;
        rep(d, 4) {
            int nx = from_x + dx[d], ny = from_y + dy[d];
            if (inside(nx, ny, w, h) && b[nx][ny] == 'T') {
                nearWall = true;
                cnt++;
            }
        }
        if (!nearWall) continue;
        if (cnt >= 2) continue;

        (void)try_wall_extend(b, from_x, from_y, sx, sy, gx, gy, rng, curr_len);
    }
}
// ---- 追記ここまで ----

// ゴール周囲の開口を 1 本に絞る。ただし「壁際に近い」&「S から遠ざかる向き」を優先して残す。
// b は b[x][y] でアクセス（あなたのコード準拠）。S/G は '.' 前提。
// 重み w_edge: 壁際バイアス（壁に近いほど高得点）、w_away: S から遠ざかる向きバイアス。
void narrow_goal_biased_toward_wall_away_from_start(vector<vector<char>> &b, int sx, int sy, // Start
                                                    int gx, int gy,                          // Goal
                                                    double w_edge = 1.0, double w_away = 2.5) {
    const int N    = (int)b.size();
    auto inside2   = [&](int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; };
    auto dist_edge = [&](int x, int y) { return min(min(x, N - 1 - x), min(y, N - 1 - y)); };

    struct Cand {
        int x, y, dir;
        double score;
        int edge;
        int dot;
        bool cornerable;
    };
    vector<Cand> cand;

    for (int d = 0; d < 4; ++d) {
        int ex = gx + (int)dx[d], ey = gy + (int)dy[d]; // entrance (Gの隣)
        if (!inside2(ex, ey)) continue;
        if (b[ex][ey] != '.') continue;

        int e             = dist_edge(ex, ey);
        double edge_score = (double)(N - e);

        int ux = ex - gx, uy = ey - gy; // G→入口
        int vx = sx - gx, vy = sy - gy; // G→S
        int dot           = ux * vx + uy * vy;
        double away_score = (double)(-dot);

        // 入口の左右どちらかが既に'.'なら角を作れる見込み
        int left  = (d + 3) & 3;
        int right = (d + 1) & 3;
        int lx = ex + (int)dx[left], ly = ey + (int)dy[left];
        int rx = ex + (int)dx[right], ry = ey + (int)dy[right];
        bool hasLeftOpen  = inside2(lx, ly) && b[lx][ly] == '.';
        bool hasRightOpen = inside2(rx, ry) && b[rx][ry] == '.';
        bool cornerable   = hasLeftOpen || hasRightOpen;

        double sc = w_edge * edge_score + w_away * away_score;
        cand.push_back({ex, ey, d, sc, e, dot, cornerable});
    }

    if ((int)cand.size() <= 1) return; // 0 or 1 開口なら何もしない

    // 角を作れる候補を優先しつつ高スコア順
    sort(cand.begin(), cand.end(), [&](const Cand &a, const Cand &b) {
        if (a.cornerable != b.cornerable) return a.cornerable > b.cornerable;
        if (a.score != b.score) return a.score > b.score;
        if (a.edge != b.edge) return a.edge < b.edge;
        if (a.dot != b.dot) return a.dot < b.dot;
        return a.dir < b.dir;
    });

    // 変更の一時適用＆ロールバック
    vector<pair<int, int>> touched;
    auto placeWall = [&](int x, int y) {
        if (!inside2(x, y)) return;
        if ((x == sx && y == sy) || (x == gx && y == gy)) return; // S/Gは触らない
        if (b[x][y] == '.') {
            b[x][y] = 'T';
            touched.emplace_back(x, y);
        }
    };
    auto rollback = [&]() {
        for (auto &p : touched) b[p.first][p.second] = '.';
        touched.clear();
    };

    // ===== T字を許さない“L字のみ”の強制 =====
    for (const auto &k : cand) {
        int d  = k.dir;         // G→入口 方向
        int ex = k.x, ey = k.y; // entrance

        // 入口の左右どちらかを開け、他方と直進は閉じる（入口側のT字禁止）
        array<int, 2> sides = {(d + 3) & 3, (d + 1) & 3}; // 左/右
        auto away_key       = [&](int sdir) {
            int sxv = sx - ex, syv = sy - ey; // 入口→S
            int dvx = (int)dx[sdir], dvy = (int)dy[sdir];
            return sxv * dvx + syv * dvy; // 小さい(負)ほどSから遠ざかる
        };
        // Sから遠ざかる曲がりを優先
        if (away_key(sides[0]) > away_key(sides[1])) swap(sides[0], sides[1]);

        for (int sidx = 0; sidx < 2; ++sidx) {
            int sdir = sides[sidx]; // これが“曲がる”方向（入口から見て左 or 右）
            int ox = ex + (int)dx[sdir], oy = ey + (int)dy[sdir]; // 曲がり角セル
            if (!inside2(ox, oy) || b[ox][oy] != '.') continue;   // 掘らない方針

            touched.clear();

            // (1) Gの他3方向を壁化（3面壁）
            for (int dd = 0; dd < 4; ++dd) {
                if (dd == d) continue; // d=入口側は残す
                int nx = gx + (int)dx[dd], ny = gy + (int)dy[dd];
                placeWall(nx, ny);
            }

            // (2) 入口の直進（Gからさらに離れる方向）を壁化 → 入口で必ず曲がる
            int fx = ex + (int)dx[d], fy = ey + (int)dy[d];
            placeWall(fx, fy);

            // (3) 入口の反対側（もう一方の横）を壁化 → 入口でT字禁止
            int tdir = sides[1 - sidx];
            int tx = ex + (int)dx[tdir], ty = ey + (int)dy[tdir];
            placeWall(tx, ty);

            // (4) 曲がり角セルの側方を**両方**壁化 → 曲がり角でT字禁止
            //     角セル(ox,oy)の開放は ex(=後方) と pre(=前方sdir) のみになる
            int l2 = (sdir + 3) & 3;
            int r2 = (sdir + 1) & 3;
            int cx = ox + (int)dx[l2], cy = oy + (int)dy[l2];
            placeWall(cx, cy);
            cx = ox + (int)dx[r2];
            cy = oy + (int)dy[r2];
            placeWall(cx, cy);

            // 到達性チェック
            if (path_exists_to_G(b, sx, sy, gx, gy)) {
                return; // 成功
            }
            rollback(); // ダメなら他サイドを試す
        }
        // この入口方向では作れなかった → 次の候補へ
    }

    // ===== フォールバック：入口一本化のみ（従来） =====
    {
        auto keep = cand.front();
        for (size_t i = 1; i < cand.size(); ++i) {
            int nx = cand[i].x, ny = cand[i].y;
            if ((nx == sx && ny == sy) || (nx == gx && ny == gy)) continue;
            char old = b[nx][ny];
            if (old != '.') continue;
            b[nx][ny] = 'T';
            if (!path_exists_to_G(b, sx, sy, gx, gy)) { b[nx][ny] = old; }
        }
    }
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    int N;
    int gx, gy;
    cin >> N >> gx >> gy;
    int sx = 0, sy = N / 2; // 入口

    vector<vector<char>> b(N, vector<char>(N));
    rep(i, N) rep(j, N) cin >> b[i][j];
    vector<vector<char>> origin = b;

    // ゴールをかこう
    narrow_goal_biased_toward_wall_away_from_start(b, sx, sy, gx, gy, /*w_edge=*/1.0, /*w_away=*/2.5);

    // 壁伸ばし法
    wall_extend(b, /*S*/ sx, sy, /*G*/ gx, gy, /*iters*/ 3000, /*seed*/ 114514ULL);
    vector<pll> ans;

    rep(i, N) rep(j, N) {
        if (b[i][j] != origin[i][j]) ans.emplace_back(i, j);
    }
    // rep(i, N) {
    //     rep(j, N) { cerr << b[i][j]; }
    //     cerr << endl;
    // }
    bool first = true;

    while (true) {
        ll x, y;
        if (!(cin >> x >> y)) break;
        if (x == gx && y == gy) break;

        ll n;
        if (!(cin >> n)) break;
        vec xs(n), ys(n);
        rep(i, n) cin >> xs[i] >> ys[i];

        if (first) {
            first = false;
            cout << ans.size();
            for (auto [x, y] : ans) cout << ' ' << x << ' ' << y;
            cout << endl;
        } else {
            cout << 0 << endl;
        }
    }
}
