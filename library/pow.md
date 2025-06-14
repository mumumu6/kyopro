# 累乗

## modpow

```cpp
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
```

## pow

```cpp
long long pow(long long a, long long n, long long mod) {
    long long res = 1;
    while (n > 0) {
        if (n & 1) res = res * a;
        a = a * a ;
        n >>= 1;
    }
    return res;
}
```
