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
