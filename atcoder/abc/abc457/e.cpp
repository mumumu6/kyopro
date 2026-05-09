#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld     = long double;
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
#define each1(i, a) for (auto &&i : a)
#define each2(x, y, a) for (auto &&[x, y] : a)
#define each3(x, y, z, a) for (auto &&[x, y, z] : a)
#define each4(w, x, y, z, a) for (auto &&[w, x, y, z] : a)
#define each(...) overload5(__VA_ARGS__, each4, each3, each2, each1)(__VA_ARGS__)
// #define sum(...) accumulate(all(__VA_ARGS__),0LL)
// #define dsum(...) accumulate(all(__VA_ARGS__),0.0L)
// #define Msum(...) accumulate(all(__VA_ARGS__),mint{})

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

ll l_op(ll a, ll b) { return std::min(a, b); }
ll l_e() { return INF; }
ll r_op(ll a, ll b) { return std::max(a, b); }
ll r_e() { return -INF; }

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m;
    cin >> n >> m;

    vecc l(n); // 左側がiにあるやつの右側
    vecc r(n); // 右側がiにあるやつの左側

    vec a(n, INF);  // 左のみてそれのの一番左
    vec b(n, -INF); // 右の見てそれの一番右
    map<pll, ll> cnt;
    rep(i, m) {
        ll x, y;
        cin >> x >> y;
        x--;
        y--;
        cnt[{x, y}]++;
        l[x].pb(y);
        r[y].pb(x);
        chmin(a[x], y);
        chmax(b[y], x);
    }

    segtree<ll, l_op, l_e> l_seg(a);
    segtree<ll, r_op, r_e> r_seg(b);

    rep(i, n) {
        so(l[i]);
        so(r[i]);
    }

    ll q;
    cin >> q;

    rep(qi, q) {
        ll s, t;
        cin >> s >> t;
        s--;
        t--;

        if (l[s].size() == 0 || r[t].size() == 0) {
            No;
            continue;
        }

        auto x = upper_bound(all(l[s]), t);
        auto y = lower_bound(all(r[t]), s);
        // ll y = r_seg.prod(s, t); // migi
        // debug(x, y, s, t);

        if (x == l[s].begin() || y == r[t].end()) {
            No;
            // debug(s, t);

            // cout << a << " " << b << endl;
            continue;
        }

        ll a = *prev(x);
        // debug(s, t, a, *y);
        // if (a > t || *y < s) {
        //     No;
        //     continue;
        // }

        if (*prev(x) == t && *y == s) {
            if (cnt[{s, t}] > 1) {
                Yes;
                continue;
            } else if (cnt[{s, t}] == 1) {
                ll c = r_seg.prod(s, t);
                if (c >= s) {
                    Yes;
                    continue;
                }
                c = l_seg.prod(s + 1, t + 1);
                if (c <= t) {
                    Yes;
                    continue;
                }
            }

            No;
            continue;
        }

        // if (a == t && b == s) {

        if (*prev(x) + 1 >= *y) Yes;
        else No;
    }
}