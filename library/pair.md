# pairのもろもろ

```cpp
template<class T> using P = pair<T, T>;

#define ft first
#define sd second

template<class T> P<T> operator+(const P<T>& a, const P<T>& b) { return {a.ft + b.ft, a.sd + b.sd}; }
template<class T> P<T> operator-(const P<T>& a, const P<T>& b) { return {a.ft - b.ft, a.sd - b.sd}; }
template<class T> P<T> operator-(const P<T>& a) { return {-a.ft, -a.sd}; }

template<class T, class U> P<T> operator*(const P<T>& a, const U& b) { return {a.ft * b, a.sd * b}; }
template<class T, class U> P<T> operator/(const P<T>& a, const U& b) { return {a.ft / b, a.sd / b}; }

template<class T> P<T>& operator+=(P<T>& a, const P<T>& b) { return a = a + b; }
template<class T> P<T>& operator-=(P<T>& a, const P<T>& b) { return a = a - b; }
template<class T, class U> P<T>& operator*=(P<T>& a, const U& b) { return a = a * b; }
template<class T, class U> P<T>& operator/=(P<T>& a, const U& b) { return a = a / b; }

template<class T> P<T> rotate(const P<T>& a) { return {-a.sd, a.ft}; } // 90 degree ccw

template<class T> T dot(const P<T>& a, const P<T>& b) { return a.ft * b.ft + a.sd * b.sd; } // 内積
template<class T> T cross(const P<T>& a, const P<T>& b) { return dot(rotate(a), b); }
template<class T> T square(const P<T>& a) { return dot(a, a); }

template<class T> ld norm(const P<T>& a) { return hypotl(a.ft, a.sd); } // aの長さが出る

template<class T> T gcd(const P<T>& a) { return gcd(a.ft, a.sd); }

template<class T> P<T> normalize(P<T> a) { // (4,6)　-> (2,3) みたいな感じ
    if(a == P<T>{}) return a;
    a /= gcd(a);
    if(a < P<T>{}) a = -a;
    return a;
}
```
例
```cpp
int main() {
    P<ll> a = {1, 2};
    P<ll> b = {3, 4};

    auto c = a + b;   // {4, 6}
    auto d = b - a;   // {2, 2}
    auto e = -a;      // {-1, -2}

    ll d2 = square(a - b); // 二乗距離

    auto f = a * 3;   // {3, 6}
    auto g = b / 2;   // {1, 2}  // ll なので整数除算

    a += b;           // a = {4, 6}

    cout << c.ft << " " << c.sd << endl;
}
```

## 黄金比分割

```cpp
ld golden_section_search(auto f, ld a, ld b) {
    const ld PHI_1 = (sqrtl(5) - 1) / 2;
    ld x = lerp(b, a, PHI_1), fx = f(x), fy;
    rep(i,60) {
        ld y = lerp(a, b, PHI_1);
        fy = f(y);
        if (fx < fy) {
            b = a;
            a = y;
        } else {
            a = x;
            x = y;
            fx = fy;
        }
    }
    return fx;
}
```
