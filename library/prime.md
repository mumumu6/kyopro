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

long long find_factor

```

## 線形篩

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
