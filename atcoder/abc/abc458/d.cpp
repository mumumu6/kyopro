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

// #define DEBUG

#ifdef DEBUG
#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll x, q;
    cin >> x >> q;

    priority_queue<ll, vector<ll>, greater<ll>> up; // 小さいのから
    priority_queue<ll, vector<ll>> down;            // 大きいのから

    rep(i, q) {
        ll a, b;
        cin >> a >> b;
        if (a > x) up.push(a);
        else down.push(a);

        if (b > x) up.push(b);
        else down.push(b);

        if (up.size() == (i + 1)) cout << x << endl;
        else if (up.size() == i) {
            ll mid = down.top();
            down.pop();

            debug(x, mid, i);
            if (x < mid) swap(x, mid); // midのほうが小さい

            up.push(x);

            cout << mid << endl;
            x = mid;

        } else {
            ll mid = up.top();
            up.pop();
            debug(x, mid);

            if (x < mid) swap(x, mid); // midのほうが小さい

            down.push(mid);
            cout << x << endl;
        }
    }
}