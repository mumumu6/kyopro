#include <bits/stdc++.h>
using namespace std;
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

long long modpow(long long a, long long n, long long mod) {
    long long res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = (i128)res * a % mod;
        a = (i128)a * a % mod;
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
        while (x != 1 and x != n - 1 and r--) x = ((i128)x * x) % n;
        if (x != n - 1 && r != s) return 0;
    }

    return true;
}

long long pollard(long long N) {
    if (N % 2 == 0) return 2;
    if (is_prime(N)) return N;

    long long step = 0;
    auto f         = [&](long long x) -> long long { return (__int128_t(x) * x + step) % N; };
    while (true) {
        ++step;
        long long x = step, y = f(x);
        while (true) {
            long long p = gcd(y - x + N, N);
            if (p == 0 || p == N) break;
            if (p != 1) return p;
            x = f(x);
            y = f(f(y));
        }
    }
}

vector<long long> factor(long long n) {
    if (n <= 1) return {}; // 1以下は素因数分解しない
    if (is_prime(n)) return {n};
    ll x = pollard(n);
    if (x == n) return {x};
    auto l = factor(x), r = factor(n / x);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}

vector<long long> get_divisors(long long n) {
    auto prime_factors = factor(n);

    map<long long, int> pf_count;
    for (auto p : prime_factors) { pf_count[p]++; }

    vector<long long> divisors = {1};
    for (auto [prime, count] : pf_count) {
        int old_size = divisors.size();
        for (int i = 0; i < old_size; i++) {
            long long power = prime;
            for (int j = 1; j <= count; j++) {
                divisors.push_back(divisors[i] * power);
                power *= prime;
            }
        }
    }

    sort(divisors.begin(), divisors.end());
    return divisors;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n = 101010;

    rep(i, n) {
        auto divisors = get_divisors(i);
        // cout << "Divisors of " << i << ": ";
        // for (auto d : divisors) { cout << d << " "; }
        // cout << el;
    }
}