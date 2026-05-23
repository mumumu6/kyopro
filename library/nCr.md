# nCr (n 個から k 個を選ぶ組み合わせ)

nやrがmodの値を超えるような場合は注意　上手く動かない
```cpp
mint fact(ll n, bool inv = false) {
    if (n < 0) return 0;

    static vector<mint> factorial = {1};
    static vector<mint> factorial_inv = {1};

    ll siz = factorial.size();

    if (n >= siz) {
        for (ll i = siz; i <= n; i++) {
            factorial.emplace_back(factorial[i - 1] * i);
        }

        factorial_inv.resize(n + 1);
        factorial_inv[n] = factorial[n].inv();

        for (ll i = n; i > siz; i--) {
            factorial_inv[i - 1] = factorial_inv[i] * i;
        }
    }

    return inv ? factorial_inv[n] : factorial[n];
}

mint comb(ll n, ll r) {
    if (r < 0 || n < r) return 0;
    if (r == 0 || r == n) return 1;

    r = min(r, n - r);

    const ll FACT_LIMIT = 6e5;

    // n が小さいなら階乗テーブルで O(1)
    if (n <= FACT_LIMIT) {
        return fact(n) * fact(r, true) * fact(n - r, true);
    }

    // n が大きいなら O(r)
    // C(n, r) = n(n-1)...(n-r+1) / r!
    mint num = 1;
    mint den = 1;

    for (ll i = 1; i <= r; i++) {
        num *= mint(n - i + 1);
        den *= mint(i);
    }

    return num / den;
}

mint perm(ll n, ll r) {
    if (r < 0 || n < r) return 0;
    if (r == 0) return 1;

    const ll FACT_LIMIT = 6e5;

    // n が小さいなら階乗テーブルで O(1)
    if (n <= FACT_LIMIT) {
        return fact(n) * fact(n - r, true);
    }

    // n が大きいなら O(r)
    // P(n, r) = n(n-1)...(n-r+1)
    mint res = 1;

    for (ll i = 0; i < r; i++) {
        res *= mint(n - i);
    }

    return res;
}

mint nHk(ll n, ll k) {
    if (n < 0 || k < 0) return 0;
    if (n == 0) return k == 0 ? 1 : 0;

    return comb(n + k - 1, k);
}
```