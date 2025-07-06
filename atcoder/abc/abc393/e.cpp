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

struct LinearSieve {
    vector<ll> primes, spf;

    LinearSieve(ll n) : spf(n + 1) {
        for (ll i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                primes.push_back(i);
                spf[i] = i;
            }
            for (ll p : primes) {
                if (i * p > n || p > spf[i]) break;
                spf[i * p] = p;
            }
        }
    }

    // 素因数分解
    map<ll, ll> factorize(ll n) {
        map<ll, ll> factors;
        while (n > 1) {
            factors[spf[n]]++;
            n /= spf[n];
        }
        return factors;
    }

    // 約数列挙
    vector<ll> divisors(ll n) {
        auto factors   = factorize(n);
        vector<ll> res = {1};

        for (auto [p, cnt] : factors) {
            ll sz = res.size();
            for (ll i = 0; i < sz; i++) {
                ll power = p;
                for (ll j = 0; j < cnt; j++) {
                    res.push_back(res[i] * power);
                    power *= p;
                }
            }
        }

        sort(res.begin(), res.end());
        return res;
    }

    // 約数の個数だけ欲しい場合
    ll count_divisors(ll n) {
        auto factors = factorize(n);
        ll res       = 1;
        for (auto [p, cnt] : factors) { res *= (cnt + 1); }
        return res;
    }
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k;
    cin >> n >> k;

    vec a(n);
    rep(i, n) cin >> a[i];

    LinearSieve sieve(2e6);
    const vec &primes = sieve.primes;

    vec cnt(2e6, 0);
    vec ans(2e6, 0);

    rep(i, n) cnt[a[i]]++;

    for (ll p : primes) {
        rep(i, 2e6 / p) {
            ll j = 2e6 / p - 1 - i;
            cnt[j] += cnt[j * p];
        }
    }

    rep(i, 2e6) {
        if (cnt[i] >= k) { ans[i] = i; }
    }

    for (ll p : primes) {
        reps(i, 1, 2e6 / p) { chmax(ans[p * i], ans[i]); }
    }

    for (ll x : a) { cout << ans[x] << el; }
}