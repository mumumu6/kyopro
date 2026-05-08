# seg木

```cpp
template <typename T> struct SegmentTree {
    ll n;
    vector<T> data;
    T e;
    function<T(T, T)> f;
    SegmentTree(ll n, T e, function<T(T, T)> f) : n(bit_ceil((unsigned)n)), e(e), f(f) {
        data.assign(2 * this->n, e);
    }

    T query(ll l, ll r) { return _inner_query(l, r, 1, 0, n); } // [l, r)

    void update(ll i, T x) {
        i += n;
        data[i] = x;
        while (i > 1) {
            i >>= 1;
            data[i] = f(data[i << 1 | 0], data[i << 1 | 1]);
        }
    }

  private:
    T _inner_query(ll l, ll r, ll i, ll il, ll ir) {
        if (r <= il || ir <= l) return e;
        if (l <= il && ir <= r) return data[i];
        int m = (il + ir) / 2;
        return f(_inner_query(l, r, i << 1 | 0, il, m), _inner_query(l, r, i << 1 | 1, m, ir));
    }
};
```

# 遅延seg木

```cpp
template <typename T>
struct LazySegmentTree {
    ll n;
    vector<T> data, lazy;
    T e;

    LazySegmentTree(ll sz, T e) : e(e) {
        n = bit_ceil((unsigned)sz);
        data.assign(2 * n, e);
        lazy.assign(2 * n, 0);
    }

    void build(const vector<T>& a) {
        for (ll i = 0; i < (ll)a.size(); i++) {
            data[n + i] = a[i];
        }
        for (ll i = n - 1; i >= 1; i--) {
            data[i] = min(data[i << 1], data[i << 1 | 1]);
        }
    }

    void apply(ll i, T x) {
        data[i] += x;
        lazy[i] += x;
    }

    void push(ll i) {
        if (lazy[i] == 0) return;
        apply(i << 1, lazy[i]);
        apply(i << 1 | 1, lazy[i]);
        lazy[i] = 0;
    }

    void range_add(ll l, ll r, T x) {
        range_add(l, r, x, 1, 0, n);
    }

    T range_min(ll l, ll r) {
        return range_min(l, r, 1, 0, n);
    }

  private:
    void range_add(ll l, ll r, T x, ll i, ll il, ll ir) {
        if (r <= il || ir <= l) return;

        if (l <= il && ir <= r) {
            apply(i, x);
            return;
        }

        push(i);

        ll mid = (il + ir) / 2;
        range_add(l, r, x, i << 1, il, mid);
        range_add(l, r, x, i << 1 | 1, mid, ir);

        data[i] = min(data[i << 1], data[i << 1 | 1]);
    }

    T range_min(ll l, ll r, ll i, ll il, ll ir) {
        if (r <= il || ir <= l) return e;

        if (l <= il && ir <= r) {
            return data[i];
        }

        push(i);

        ll mid = (il + ir) / 2;
        return min(
            range_min(l, r, i << 1, il, mid),
            range_min(l, r, i << 1 | 1, mid, ir)
        );
    }
};
```