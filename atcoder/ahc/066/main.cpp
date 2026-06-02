#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld     = long double;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
template <class T, class U> bool chmin(T &a, const U &b) { return a > b ? a = b, 1 : 0; }
template <class T, class U> bool chmax(T &a, const U &b) { return a < b ? a = b, 1 : 0; }
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }

// pair操作
template <class T> using P = pair<T, T>;
#define ft first
#define sd second
vector<ll> dx8 = {0, 1, 1, 1, 0, -1, -1, -1}, dy8 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
vector<P<ll>> pdx4 = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
vector<P<ll>> pdx8 = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
template <class T> P<T> operator+(const P<T> &a, const P<T> &b) { return {a.ft + b.ft, a.sd + b.sd}; }
template <class T> P<T> operator-(const P<T> &a, const P<T> &b) { return {a.ft - b.ft, a.sd - b.sd}; }
template <class T> P<T> operator-(const P<T> &a) { return {-a.ft, -a.sd}; }
template <class T, class U> P<T> operator*(const P<T> &a, const U &b) { return {a.ft * b, a.sd * b}; }
template <class T, class U> P<T> operator/(const P<T> &a, const U &b) { return {a.ft / b, a.sd / b}; }
template <class T> P<T> &operator+=(P<T> &a, const P<T> &b) { return a = a + b; }
template <class T> P<T> &operator-=(P<T> &a, const P<T> &b) { return a = a - b; }
template <class T, class U> P<T> &operator*=(P<T> &a, const U &b) { return a = a * b; }
template <class T, class U> P<T> &operator/=(P<T> &a, const U &b) { return a = a / b; }
template <class T> P<T> rotate(const P<T> &a) { return {-a.sd, a.ft}; } // 90 degree ccw
template <class T> T dot(const P<T> &a, const P<T> &b) { return a.ft * b.ft + a.sd * b.sd; } // 内積
template <class T> T cross(const P<T> &a, const P<T> &b) { return dot(rotate(a), b); }
template <class T> T square(const P<T> &a) { return dot(a, a); }
template <class T> ld norm(const P<T> &a) { return hypotl(a.ft, a.sd); } // aの長さが出る
template <class T> T gcd(const P<T> &a) { return gcd(a.ft, a.sd); }
template <class T> P<T> normalize(P<T> a) { // (4,6)　-> (2,3) みたいな感じ
    if (a == P<T>{}) return a;
    a /= gcd(a);
    if (a < P<T>{}) a = -a;
    return a;
}

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
#define overload5(a, b, c, d, e, name, ...) name
#define each1(i, a) for (auto &&i : a)
#define each2(x, y, a) for (auto &&[x, y] : a)
#define each3(x, y, z, a) for (auto &&[x, y, z] : a)
#define each4(w, x, y, z, a) for (auto &&[w, x, y, z] : a)
#define each(...) overload5(__VA_ARGS__, each4, each3, each2, each1)(__VA_ARGS__)
// #define sum(...) accumulate(all(__VA_ARGS__),0LL)
// #define dsum(...) accumulate(all(__VA_ARGS__),0.0L)
// #define Msum(...) accumulate(all(__VA_ARGS__),mint{})

struct Inside {
    ll h, w;
    Inside(ll h, ll w) : h(h), w(w) {}
    Inside(P<ll> hw) : h(hw.ft), w(hw.sd) {}
    bool operator()(ll i, ll j) const { return 0 <= i && i < h && 0 <= j && j < w; }
    template <class T> bool operator()(P<T> p) const { return (*this)(p.ft, p.sd); }
};

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

template <class T> ostream &operator<<(ostream &os, const vector<vector<T>> &vv) {
    os << "[\n";
    for (ll i = 0; i < (ll)vv.size(); ++i) {
        os << "  " << vv[i];
        if (i + 1 < (ll)vv.size()) os << '\n';
    }
    return os << "\n]";
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

// これをon offする
#define DEBUG

#ifdef DEBUG
#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif

string expandMacro(const string &compressed) {
    string result;
    string last_macro;
    string recording_macro;
    bool recording = false;

    auto append_ops = [&](const string &ops) {
        result += ops;
        if (recording) recording_macro += ops;
    };

    for (char c : compressed) {
        if (c == 'F' || c == 'R' || c == 'L' || c == 'S') {
            append_ops(string(1, c));
        } else if (c == 'M') {
            if (!recording) {
                recording       = true;
                recording_macro = "";
            } else {
                recording  = false;
                last_macro = recording_macro;
            }
        } else if (c == 'P') {
            append_ops(last_macro);
        } else {
            assert(false);
        }
    }
    return result;
}

string compressOneMacro(const string &ops) {
    const int n       = (int)ops.size();
    const int max_len = min(200, n);

    struct Candidate {
        int gain  = 0;
        int len   = 0;
        int first = INT_MAX;
        string s;
    };

    Candidate best;

    for (int len = 3; len <= max_len; ++len) {
        unordered_map<string, vector<int>> pos;
        pos.reserve(max(1, n - len + 1) * 2);

        for (int i = 0; i + len <= n; ++i) { pos[ops.substr(i, len)].push_back(i); }

        for (auto &[s, positions] : pos) {
            int count    = 0;
            int last_end = -1;
            int first    = -1;
            for (int p : positions) {
                if (p >= last_end) {
                    if (first == -1) first = p;
                    ++count;
                    last_end = p + len;
                }
            }
            if (count < 2) continue;

            int gain = (count - 1) * (len - 1) - 2;
            if (gain > best.gain ||
                (gain == best.gain && (len > best.len || (len == best.len && first < best.first)))) {
                best = {gain, len, first, s};
            }
        }
    }

    if (best.gain <= 0) return ops;

    string compressed;
    compressed.reserve(n - best.gain);

    const int first = best.first;
    const int len   = best.len;
    const string &s = best.s;

    compressed += ops.substr(0, first);
    compressed += 'M';
    compressed += s;
    compressed += 'M';

    for (int i = first + len; i < n;) {
        if (i + len <= n && ops.compare(i, len, s) == 0) {
            compressed += 'P';
            i += len;
        } else {
            compressed += ops[i];
            ++i;
        }
    }

    assert(expandMacro(compressed) == ops);
    assert((int)compressed.size() == n - best.gain);
    return compressed;
}

struct OrderOptimizer {
    static constexpr ll INF_DIST = 1'000'000'000;
    static constexpr ll INF_COST  = (1LL << 60);
    using Point = P<ll>;

    ll N, M;
    Inside inside;
    vector<Point> ball;
    vector<Point> basket;
    function<bool(ll, ll, ll)> can_move_func;

    // 重要点: 0 = start, 1..M = ball, 1+M..2M = basket
    vector<Point> important;
    vector<vector<ll>> dist_from_important;
    vector<vector<ll>> important_dist;

    OrderOptimizer(ll N, ll M, const vector<Point> &ball, const vector<Point> &basket,
                   function<bool(ll, ll, ll)> can_move_func = {})
        : N(N), M(M), inside(N, N), ball(ball), basket(basket), can_move_func(can_move_func) {
        assert((ll)ball.size() == M);
        assert((ll)basket.size() == M);
        buildDistances();
    }

    bool canMove(ll i, ll j, ll dir) const {
        static const ll di[4] = {-1, 0, 1, 0};
        static const ll dj[4] = {0, 1, 0, -1};
        ll ni = i + di[dir], nj = j + dj[dir];
        if (!inside(Point{i, j}) || !inside(Point{ni, nj})) return false;
        if (can_move_func) return can_move_func(i, j, dir);
        return true;
    }

    ll pointIdStart() const { return 0; }
    ll pointIdBall(ll k) const { return 1 + k; }
    ll pointIdBasket(ll k) const { return 1 + M + k; }

    vector<ll> bfsFrom(Point src) const {
        static const ll di[4] = {-1, 0, 1, 0};
        static const ll dj[4] = {0, 1, 0, -1};
        vector<ll> dist(N * N, INF_DIST);
        queue<Point> q;
        dist[src.ft * N + src.sd] = 0;
        q.push(src);

        while (!q.empty()) {
            auto [i, j] = q.front();
            q.pop();
            ll d = dist[i * N + j];
            rep(dir, 4) {
                if (!canMove(i, j, dir)) continue;
                ll ni = i + di[dir], nj = j + dj[dir];
                ll id = ni * N + nj;
                if (dist[id] != INF_DIST) continue;
                dist[id] = d + 1;
                q.push({ni, nj});
            }
        }
        return dist;
    }

    void buildDistances() {
        important.clear();
        important.push_back({0, 0});
        for (auto p : ball) important.push_back(p);
        for (auto p : basket) important.push_back(p);

        ll K = (ll)important.size();
        dist_from_important.assign(K, {});
        important_dist.assign(K, vector<ll>(K, INF_DIST));

        rep(i, K) {
            dist_from_important[i] = bfsFrom(important[i]);
            rep(j, K) {
                auto [r, c] = important[j];
                important_dist[i][j] = dist_from_important[i][r * N + c];
            }
        }
    }

    ll distPoint(ll from_id, ll to_id) const { return important_dist[from_id][to_id]; }
    ll distStartBall(ll k) const { return distPoint(pointIdStart(), pointIdBall(k)); }
    ll distBallBasket(ll k) const { return distPoint(pointIdBall(k), pointIdBasket(k)); }
    ll distBasketBall(ll a, ll b) const { return distPoint(pointIdBasket(a), pointIdBall(b)); }

    ll addCost(ll a, ll b) const {
        if (a >= INF_COST || b >= INF_DIST) return INF_COST;
        return min(INF_COST, a + b);
    }

    bool isPermutation(const vector<ll> &order) const {
        if ((ll)order.size() != M) return false;
        vector<ll> seen(M, 0);
        for (ll x : order) {
            if (x < 0 || x >= M || seen[x]) return false;
            seen[x] = 1;
        }
        return true;
    }

    ll calcOrderCost(const vector<ll> &order) const {
        assert(isPermutation(order));
        if (order.empty()) return 0;

        ll cost = 0;
        cost = addCost(cost, distStartBall(order[0]));
        for (ll k : order) cost = addCost(cost, distBallBasket(k));
        rep(i, (ll)order.size() - 1) {
            cost = addCost(cost, distBasketBall(order[i], order[i + 1]));
        }
        return cost;
    }

    vector<ll> makeInitialOrderNumbered() const {
        vector<ll> order(M);
        iota(all(order), 0LL);
        return order;
    }

    ll distCurToBall(ll cur_basket_task, ll k) const {
        if (cur_basket_task == -1) return distStartBall(k);
        return distBasketBall(cur_basket_task, k);
    }

    vector<ll> makeInitialOrderNearestPickup() const {
        vector<ll> order;
        vector<char> used(M, false);
        ll cur = -1;
        order.reserve(M);

        rep(step, M) {
            ll best = -1;
            ll best_score = INF_DIST;
            rep(k, M) {
                if (used[k]) continue;
                ll score = distCurToBall(cur, k);
                if (score < best_score || (score == best_score && k < best)) {
                    best_score = score;
                    best = k;
                }
            }
            assert(best != -1);
            used[best] = true;
            order.push_back(best);
            cur = best;
        }
        assert(isPermutation(order));
        return order;
    }

    vector<ll> makeInitialOrderPickupDelivery() const {
        vector<ll> order;
        vector<char> used(M, false);
        ll cur = -1;
        order.reserve(M);

        rep(step, M) {
            ll best = -1;
            ll best_score = INF_COST;
            rep(k, M) {
                if (used[k]) continue;
                ll score = addCost(distCurToBall(cur, k), distBallBasket(k));
                if (score < best_score || (score == best_score && k < best)) {
                    best_score = score;
                    best = k;
                }
            }
            assert(best != -1);
            used[best] = true;
            order.push_back(best);
            cur = best;
        }
        assert(isPermutation(order));
        return order;
    }

    vector<ll> makeInitialOrderLookahead(double alpha) const {
        vector<ll> order;
        vector<char> used(M, false);
        ll cur = -1;
        order.reserve(M);

        rep(step, M) {
            ll best = -1;
            double best_score = 1e100;
            rep(k, M) {
                if (used[k]) continue;

                ll nxt_best = INF_DIST;
                rep(nxt, M) {
                    if (used[nxt] || nxt == k) continue;
                    chmin(nxt_best, distBasketBall(k, nxt));
                }
                if (nxt_best == INF_DIST) nxt_best = 0;

                double score = (double)distCurToBall(cur, k) + distBallBasket(k) + alpha * nxt_best;
                if (score < best_score || (score == best_score && k < best)) {
                    best_score = score;
                    best = k;
                }
            }
            assert(best != -1);
            used[best] = true;
            order.push_back(best);
            cur = best;
        }
        assert(isPermutation(order));
        return order;
    }

    vector<ll> makeBestInitialOrder() const {
        vector<vector<ll>> candidates;
        candidates.push_back(makeInitialOrderNumbered());
        candidates.push_back(makeInitialOrderNearestPickup());
        candidates.push_back(makeInitialOrderPickupDelivery());
        for (double alpha : {0.3, 0.5, 1.0}) candidates.push_back(makeInitialOrderLookahead(alpha));

        vector<ll> best = candidates[0];
        ll best_cost = calcOrderCost(best);
        for (auto &order : candidates) {
            ll cost = calcOrderCost(order);
            if (cost < best_cost) {
                best_cost = cost;
                best = order;
            }
        }
        return best;
    }

    vector<ll> optimizeOrderSA(double timeLimitSec, uint32_t seed = 1) const {
        vector<ll> cur_order = makeBestInitialOrder();
        vector<ll> best_order = cur_order;
        ll cur_cost = calcOrderCost(cur_order);
        ll best_cost = cur_cost;
        if (M <= 1 || timeLimitSec <= 0.0) return best_order;

        mt19937 rng(seed);
        uniform_real_distribution<double> real01(0.0, 1.0);
        const double startTemp = max(100.0, (double)cur_cost / max(1LL, M) * 0.2);
        const double endTemp = 1e-3;

        auto start = chrono::steady_clock::now();
        while (true) {
            double elapsed = chrono::duration<double>(chrono::steady_clock::now() - start).count();
            if (elapsed >= timeLimitSec) break;
            double progress = min(1.0, elapsed / timeLimitSec);
            double temp = startTemp * pow(endTemp / startTemp, progress);

            vector<ll> nxt_order = cur_order;
            ll type = uniform_int_distribution<ll>(0, 99)(rng);
            ll i = uniform_int_distribution<ll>(0, M - 1)(rng);
            ll j = uniform_int_distribution<ll>(0, M - 1)(rng);

            if (type < 40) {
                if (i == j) continue;
                swap(nxt_order[i], nxt_order[j]);
            } else if (type < 70) {
                if (i > j) swap(i, j);
                if (i == j) continue;
                reverse(nxt_order.begin() + i, nxt_order.begin() + j + 1);
            } else {
                if (i == j) continue;
                ll x = nxt_order[i];
                nxt_order.erase(nxt_order.begin() + i);
                nxt_order.insert(nxt_order.begin() + j, x);
            }

            ll nxt_cost = calcOrderCost(nxt_order);
            bool accept = nxt_cost <= cur_cost;
            if (!accept && temp > 0.0) {
                double prob = exp((double)(cur_cost - nxt_cost) / temp);
                accept = real01(rng) < prob;
            }
            if (!accept) continue;

            cur_order = move(nxt_order);
            cur_cost = nxt_cost;
            if (cur_cost < best_cost) {
                best_cost = cur_cost;
                best_order = cur_order;
            }
        }

        assert(isPermutation(best_order));
        return best_order;
    }
};

struct OperationBuilder {
    using Point = P<ll>;

    ll N;
    Inside inside;
    function<bool(ll, ll, ll)> can_move;
    Point pos = {0, 0};
    ll dir = 1; // 0:U, 1:R, 2:D, 3:L
    string ops;

    OperationBuilder(ll N, function<bool(ll, ll, ll)> can_move) : N(N), inside(N, N), can_move(can_move) {}

    string shortestMoveOps(Point start, ll start_dir, Point goal) const {
        static const vector<Point> d = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        ll SZ = N * N * 4;
        vector<ll> dist(SZ, INF);
        vector<ll> prev(SZ, -1);
        vector<char> prev_op(SZ, 0);
        queue<pll> q;

        auto id = [&](Point p, ll d) { return (p.ft * N + p.sd) * 4 + d; };
        dist[id(start, start_dir)] = 0;
        q.push({id(start, start_dir), start_dir});

        ll goal_id = -1;

        while (!q.empty()) {
            auto [state, cur_dir] = q.front();
            q.pop();
            ll cell = state / 4;
            Point p = {cell / N, cell % N};
            if (p == goal) {
                goal_id = state;
                break;
            }

            {
                ll nd = (cur_dir + 1) % 4;
                ll nid = id(p, nd);
                if (dist[nid] == INF) {
                    dist[nid] = dist[state] + 1;
                    prev[nid] = state;
                    prev_op[nid] = 'R';
                    q.push({nid, nd});
                }
            }
            {
                ll nd = (cur_dir + 3) % 4;
                ll nid = id(p, nd);
                if (dist[nid] == INF) {
                    dist[nid] = dist[state] + 1;
                    prev[nid] = state;
                    prev_op[nid] = 'L';
                    q.push({nid, nd});
                }
            }
            if (can_move(p.ft, p.sd, cur_dir)) {
                Point np = p + d[cur_dir];
                if (inside(np)) {
                    ll nid = id(np, cur_dir);
                    if (dist[nid] == INF) {
                        dist[nid] = dist[state] + 1;
                        prev[nid] = state;
                        prev_op[nid] = 'F';
                        q.push({nid, cur_dir});
                    }
                }
            }
        }

        string res;
        if (goal_id == -1) return res;
        for (ll cur = goal_id; prev[cur] != -1; cur = prev[cur]) {
            res += prev_op[cur];
        }
        reverse(all(res));
        return res;
    }

    void applyMoveOps(const string &move_ops) {
        static const vector<Point> d = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (char c : move_ops) {
            if (c == 'R') {
                dir = (dir + 1) % 4;
            } else if (c == 'L') {
                dir = (dir + 3) % 4;
            } else if (c == 'F') {
                if (can_move(pos.ft, pos.sd, dir)) pos += d[dir];
            }
            ops += c;
        }
    }

    void moveTo(Point goal) {
        string move_ops = shortestMoveOps(pos, dir, goal);
        applyMoveOps(move_ops);
        assert(pos == goal);
    }

    void swapBall() { ops += 'S'; }
};

int main() {
    auto program_start = chrono::steady_clock::now();
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m, t;
    cin >> n >> m >> t;

    vector<string> v(n); // v_ijは (i,j) と (i,j+1)
    vector<string> h(n - 1); // h_ijは (i,j) と (i+1,j)
    vector<P<ll>> ball(m), basket(m);

    rep(i, n) cin >> v[i];
    rep(i, n - 1) cin >> h[i];
    rep(k, m) {
        ll b, c, d, e;
        cin >> b >> c >> d >> e;
        ball[k] = {b, c};
        basket[k] = {d, e};
    }

    Inside in(n, n);
    auto canMove = [&](ll i, ll j, ll dir) -> bool {
        static const vector<P<ll>> d = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        P<ll> p = {i, j};
        P<ll> np = p + d[dir];
        if (!in(p) || !in(np)) return false;
        if (dir == 0) return h[i - 1][j] == '0';
        if (dir == 1) return v[i][j] == '0';
        if (dir == 2) return h[i][j] == '0';
        if (dir == 3) return v[i][j - 1] == '0';
        return false;
    };

    OrderOptimizer optimizer(n, m, ball, basket, canMove);
    double elapsed = chrono::duration<double>(chrono::steady_clock::now() - program_start).count();
    double sa_time = max(0.05, 1.75 - elapsed);
    vector<ll> order = optimizer.optimizeOrderSA(sa_time);

    OperationBuilder builder(n, canMove);
    for (ll k : order) {
        builder.moveTo(ball[k]);
        builder.swapBall();
        builder.moveTo(basket[k]);
        builder.swapBall();
    }

    string ans;
    if ((ll)builder.ops.size() <= t) {
        ans = compressOneMacro(builder.ops);
        if ((ll)ans.size() > t) ans = builder.ops;
    } else {
        ans = builder.ops.substr(0, t);
    }

    cout << ans << endl;
}
