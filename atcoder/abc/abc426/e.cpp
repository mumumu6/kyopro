#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld    = long double;
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

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

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



int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll t;
    cin >> t;

    rep(ti, t) {
        pll ts, tg;
        pll as, ag;

        cin >> ts.ft >> ts.sd >> tg.ft >> tg.sd >> as.ft >> as.sd >> ag.ft >> ag.sd;

        // 先につく方

        auto dist = [&](pll a, pll b) {
         long double dx = a.ft - b.ft;
         long double dy = a.sd - b.sd;
            return dx * dx + dy * dy;
        };

        // 距離の関数
        auto f = [&](long double time) {
            long double dxt = tg.ft - ts.ft;
            long double dyt = tg.sd - ts.sd;

            auto e = [&](long double a, long double b) { return sqrt(a * a + b * b); };

            ll  aa =  e(dxt, dyt);
            
            dxt /= aa;
            dyt /= aa;

            long double dxa = ag.ft - as.ft;
            long double dya = ag.sd - as.sd;
            ll  bb =  e(dxa, dya);
            dxa /= bb;
            dya /= bb;

            long double x = ts.ft - as.ft + time * (dxt - dxa);
            long double y = ts.sd - as.sd + time * (dyt - dya);

            return x * x + y * y;
        };

        auto g = [&](long double time) {
            long double d = (tg.ft - ts.ft) * (tg.ft - ts.ft) + (tg.sd - ts.sd) * (tg.sd - ts.sd);
            d             = sqrt(d);
            

            long double dx = ts.ft + time * (long double)(tg.ft - ts.ft) / (long double)d - ag.ft;
            long double dy = ts.sd + time * (long double)(tg.sd - ts.sd) / (long double)d - ag.sd;

            return dx * dx + dy * dy;
        };

        // ts,tgの方が遠い位置にある
        if (dist(ts, tg) < dist(as, ag)) {
            swap(ts, as);
            swap(tg, ag);
        }

        long double dist_first  = f(0);                  // はじめ
        long double dist_end    = f(sqrt(dist(as, ag))); // 片っぽだけ終り
        long double dist_finish = g(sqrt(dist(ts, tg))); // 終わり

        // 途中で最小をとるばあい
        // ll a = (tg.ft + as.ft - ts.ft - ag.ft) * (tg.ft + as.ft - ts.ft - ag.ft) +
        //        (tg.sd + as.sd - ts.sd - ag.sd) * (tg.sd + as.sd - ts.sd - ag.sd);
        // ll b = 2 * (tg.ft + as.ft - ts.ft - ag.ft) * (ts.ft - as.ft) +
        //        2 * (tg.sd + as.sd - ts.sd - ag.sd) * (ts.sd - as.sd);

        // ll c = (ts.ft - as.ft) * (ts.ft - as.ft) + (ts.sd - as.sd) * (ts.sd - as.sd);
        // debug(a, b, c);
        // debug((tg.ft + as.ft - ts.ft - ag.ft));
        // debug((tg.sd + as.sd - ts.sd - ag.sd));
        // debug((ts.ft - as.ft));

        auto f_min = [&]() {
            long double dxt = tg.ft - ts.ft;
            long double dyt = tg.sd - ts.sd;

            auto e = [&](long double a, long double b) { return sqrt(a * a + b * b); };

            dxt /= e(dxt, dyt);
            dyt /= e(dxt, dyt);

            long double dxa = ag.ft - as.ft;
            long double dya = ag.sd - as.sd;
            dxa /= e(dxa, dya);
            dya /= e(dxa, dya);

            long double a = (dxt - dxa) * (dxt - dxa) + (dyt - dya) * (dyt - dya);
            long double b = 2 * (ts.ft - as.ft) * (dxt - dxa) + 2 * (ts.sd - as.sd) * (dyt - dya);

            debug(a,b);
            debug(dxt-dxa,dyt-dya);

            return -1 * (long double)b / (long double)(2 * a);
        };

        long double min_time = f_min();
        long double min_dist          = f(min_time);

        long double ans;
        if (min_time >= 0 && min_time <= sqrt(dist(as, ag))) {
            ans = min({dist_first, dist_end, dist_finish, min_dist});
        } else {
            ans = min({dist_first, dist_end, dist_finish});
        }

        debug(dist_first, dist_end, dist_finish, min_dist);
        debug(min_time);

        cout << sqrt(ans) << endl;
    }
}