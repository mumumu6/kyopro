# その他

## 回文判定

```cpp
bool is_palindrome(const string &s) {
    int n = s.size();
    for (int i = 0; i < n / 2; i++) {
        if (s[i] != s[n - 1 - i]) return false;
    }
    return true;
}
```

## 二次元累積和
[url](https://nyaannyaan.github.io/library/data-structure-2d/2d-cumulative-sum.hpp)
```cpp
template <class T>
struct CumulativeSum2D {
  vector<vector<T> > data;

  CumulativeSum2D(int H, int W) : data(H + 3, vector<T>(W + 3, 0)) {}

  void add(int i, int j, T z) {
    ++i, ++j;
    if (i >= (int)data.size() || j >= (int)data[0].size()) return;
    data[i][j] += z;
  }

  // 半開
  void imos(int i1, int j1, int i2, int j2, T z = 1) {
    add(i1, j1, z);
    add(i1, j2, -z);
    add(i2, j1, -z);
    add(i2, j2, z);
  }

  void build() {
    for (int i = 1; i < (int)data.size(); i++) {
      for (int j = 1; j < (int)data[i].size(); j++) {
        data[i][j] += data[i][j - 1] + data[i - 1][j] - data[i - 1][j - 1];
      }
    }
  }

  // imos (i,j) を get
  T imos_get(int i, int j) { return data[i + 1][j + 1]; }

  // 半開
  T query(int i1, int j1, int i2, int j2) {
    return (data[i2][j2] - data[i1][j2] - data[i2][j1] + data[i1][j1]);
  }
};
```

例
各マスに値があるなら
```cpp
int H, W;
cin >> H >> W;

CumulativeSum2D<long long> cs(H, W);

for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
        long long x;
        cin >> x;
        cs.add(i, j, x);
    }
}

cs.build();

// [i1, i2) × [j1, j2) の和
long long ans = cs.query(i1, j1, i2, j2);
```

あるいは、各クエリで [i1, i2) × [j1, j2) に値が降られるなら

注: これは累積和ではなくimos法、累積和をしたいならもういちどbuildをするとできる。
```cpp
int H, W, Q;
cin >> H >> W >> Q;

CumulativeSum2D<long long> cs(H, W);

for (int q = 0; q < Q; q++) {
    int i1, j1, i2, j2;
    long long x;
    cin >> i1 >> j1 >> i2 >> j2 >> x;

    // [i1, i2) × [j1, j2) に x を足す
    cs.imos(i1, j1, i2, j2, x);
}

// 差分配列を実際の値に戻す
cs.build();

// 各マスの値
long long v = cs.imos_get(i, j);
```