# 素数系

## エラトステネスの篩

```cpp
vector<bool> eratos(ll n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (ll i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (ll j = i * i; j <= n; j += i) { is_prime[j] = false; }
        }
    }

    return is_prime;
}
```

## ポラード・ロー法

```cpp


// ここにミラーラビン

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
```

### ポラードローを用いた約数列挙

$n=10^5$でループ回して全部について実行したら1.3sくらい

```cpp
vector<long long> get_divisors(long long n) {
    auto prime_factors = factor(n);
    
    map<long long, int> pf_count;
    for (auto p : prime_factors) {
        pf_count[p]++;
    }
    
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
```

## 線形篩

素数判定と約数列挙ができる。
spfは最小の約数を持つ

```cpp
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
```

## ミラーラビン素数判定

(n < 2^64) ならこれで ok

```cpp
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
```

## 区間篩

```cpp
vector<long long> segment_sieve(long long a, long long b) {
    long long sqrt_b = (long long)ceil(sqrt(b));
    vector<bool> is_prime_small(sqrt_b, true);
    is_prime_small[0] = is_prime_small[1] = false;
    for (long long i = 2; i < sqrt_b; ++i) {
        if (!is_prime_small[i]) continue;
        for (long long j = i * i; j < sqrt_b; j += i) {
            is_prime_small[j] = false;
        }
    }

    vector<bool> is_prime(b - a, true);
    for (long long i = 2; i < sqrt_b; ++i) {
        if (!is_prime_small[i]) continue;
        long long k = max(i, (a + i - 1) / i);
        for (long long j = k * i; j < b; j += i) {
            is_prime[j - a] = false;
        }
    }

    vector<long long> primes;
    for (long long i = 0; i < b - a; ++i) {
        if (is_prime[i] && i + a >= 2) {
            primes.push_back(i + a);
        }
    }
    return primes;
}
```
