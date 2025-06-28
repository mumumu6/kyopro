#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
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
// ----------------- 階上 ------------------
mint fact(ll n, bool inv = false) {
    if (n < 0) return 0; // 負数は0

    static vector<mint> factorial     = {1}; // pair<階乗, 階乗の逆数>
    static vector<mint> factorial_inv = {1}; // pair<階乗, 階乗の逆数>

    ll siz = factorial.size();

    if (n >= siz) {
        reps(i, siz, n + 1) factorial.emplace_back(factorial[i - 1] * i);
        factorial_inv.resize(n + 1);
        factorial_inv[n] = factorial[n].inv(); // ここだけ逆元をとる。
        for (ll i = n; i > siz; i--) factorial_inv[i - 1] = factorial_inv[i] * i;
    }

    return inv ? factorial_inv[n] : factorial[n];
}

mint perm(ll n, ll k) { return fact(n) * fact(n - k, true); }
mint comb(ll n, ll k) { return perm(n, k) * fact(k, true); }

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
    return os << m.val();
}

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

long long modpow(long long a, long long n, long long mod) {
    long long res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}

bool is_prime(ll n) {
    if (n < 2 or n % 6 % 4 != 1) return (n | 1) == 3;

    vector<ll> bases = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    ll s = __builtin_ctzll(n - 1), d = n >> s;

    for (ll a : bases) {
        if (a % n == 0) continue;
        ll r = s;
        ll x = modpow(a % n, d, n);
        while (x != 1 and x != n - 1 and r--) x = (x * x) % n;
        if (x != n - 1 && r != s) return 0;
    }

    return true;
}

ll ans = 0;

long long pollard(long long N) {
    if (N % 2 == 0) return 2;
    if (is_prime(N)) return N;

    long long step = 0;
    auto f         = [&](long long x) -> long long { return ((x)*x + step) % N; };
    while (true) {
        ++step;
        long long x = step, y = f(x);
        while (true) {
            ans++;
            long long p = gcd(y - x + N, N);
            if (p == 0 || p == N) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}

set<long long> factor(long long n) {
    if (n <= 1) return {};
    if (is_prime(n)) return {n};
    ll x = pollard(n);
    if (x == n) return {x};
    auto l = factor(x), r = factor(n / x);
    // Insert elements from r into l one by one
    for (const auto &elem : r) { l.insert(elem); }
    return l;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll q;
    cin >> q;

    ll inf = 1010101;

    vector<ll> cnt;

    // 遅すぎて無理

    rep(x, inf + 1) {
        if (factor(x).size() == 2) cnt.pb(x * x);
    }

    cout << ans << el;

    rep(qi, q) {
        ll n;
        cin >> n;

        auto it = upper_bound(all(cnt), n);

        cout << *prev(it) << el; // nより小さい最大の素数の二乗を出力
    }
    return 0;
}