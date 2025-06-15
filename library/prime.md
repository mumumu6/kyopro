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
    if (n < 2) return false;
    if (n == 2) return true; // 2と3は素数
    if (n % 2 == 0) return false;

    vector<ll> bases = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    ll d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (auto a : bases) {
        if (a % n == 0) continue;
        a %= n;
        ll x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = true; // 合成数だったらok

        reps(r, 1, s) {


            x = (i128)(x)*x % n;
            if (x == n - 1) {
                ok = false;
                break;
            }
        }
        if (ok) return false; // 合成数
    }

    return true;
}
```
