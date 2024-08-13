#include<bits/stdc++.h>
using namespace std;


// Segment Tree
template<class Monoid, Monoid(*OP)(Monoid, Monoid), Monoid IDENTITY> struct SegTree {
    // size of segtree-array
    int N;
    
    // inner data
    int offset;
    vector<Monoid> dat;

    // constructor
    SegTree() {}
    SegTree(int n) : N(n) {
        init(n);
    }
    SegTree(const vector<Monoid> &v) : N(v.size()) {
        init(v);
    }
    void init(int n) {
        N = n;
        offset = 1;
        while (offset < N) offset *= 2;
        dat.assign(offset * 2, IDENTITY);
    }
    void init(const vector<Monoid> &v) {
        N = (int)v.size();
        offset = 1;
        while (offset < N) offset *= 2;
        dat.assign(offset * 2, IDENTITY);
        for (int i = 0; i < N; ++i) dat[i + offset] = v[i];
        build();
    }
    void build() {
        for (int k = offset - 1; k > 0; --k) dat[k] = OP(dat[k*2], dat[k*2+1]);
    }
    int size() const {
        return N;
    }
    Monoid operator [] (int a) const { return dat[a + offset]; }
    
    // update A[a], a is 0-indexed, O(log N)
    void set(int a, const Monoid &v) {
        int k = a + offset;
        dat[k] = v;
        while (k >>= 1) dat[k] = OP(dat[k*2], dat[k*2+1]);
    }
    
    // get [a, b), a and b are 0-indexed, O(log N)
    Monoid prod(int a, int b) {
        Monoid vleft = IDENTITY, vright = IDENTITY;
        for (int left = a + offset, right = b + offset; left < right;
        left >>= 1, right >>= 1) {
            if (left & 1) vleft = OP(vleft, dat[left++]);
            if (right & 1) vright = OP(dat[--right], vright);
        }
        return OP(vleft, vright);
    }
    Monoid all_prod() { return dat[1]; }
    
    // get max r that f(get(l, r)) = True (0-indexed), O(log N)
    // f(IDENTITY) need to be True
    int max_right(const function<bool(Monoid)> f, int l = 0) {
        if (l == N) return N;
        l += offset;
        Monoid sum = IDENTITY;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(OP(sum, dat[l]))) {
                while (l < offset) {
                    l = l * 2;
                    if (f(OP(sum, dat[l]))) {
                        sum = OP(sum, dat[l]);
                        ++l;
                    }
                }
                return l - offset;
            }
            sum = OP(sum, dat[l]);
            ++l;
        } while ((l & -l) != l);  // stop if l = 2^e
        return N;
    }

    // get min l that f(get(l, r)) = True (0-indexed), O(log N)
    // f(IDENTITY) need to be True
    int min_left(const function<bool(Monoid)> f, int r = -1) {
        if (r == 0) return 0;
        if (r == -1) r = N;
        r += offset;
        Monoid sum = IDENTITY;
        do {
            --r;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(OP(dat[r], sum))) {
                while (r < offset) {
                    r = r * 2 + 1;
                    if (f(OP(dat[r], sum))) {
                        sum = OP(dat[r], sum);
                        --r;
                    }
                }
                return r + 1 - offset;
            }
            sum = OP(dat[r], sum);
        } while ((r & -r) != r);
        return 0;
    }
    
    // debug
    friend ostream& operator << (ostream &s, const SegTree &seg) {
        for (int i = 0; i < seg.size(); ++i) {
            s << seg[i];
            if (i != seg.size()-1) s << " ";
        }
        return s;
    }
};

// 十分大きい値
const int INF = 1<<30;

// 演算方法
int op(int a, int b) { return max(a, b); }

int main() {
    int N, Q;
    cin >> N >> Q;
    
    // セグ木の準備 (型: int, 演算方法: op, 単位元: -INF)
    SegTree<int, op, -INF> seg(N);
    
    // 初期状態では数列 A の値はすべて 0 であるため、各要素を 0 にする
    for (int i = 0; i < N; ++i) seg.set(i, 0);
    
    // クエリ処理
    while (Q--) {
        int t;
        cin >> t;
        if (t == 1) {
            int pos, x;
            cin >> pos >> x;
            --pos;
            
            // A[pos] を x に更新する処理
            seg.set(pos, x);
        } else {
            int l, r;
            cin >> l >> r;
            --l, --r;
            
            // A の区間 [l, r) の最大値を求める処理
            cout << seg.prod(l, r) << endl;
        }
    }
}