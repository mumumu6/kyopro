# 忘れそうな書き方

テンプレート 1

```cpp
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)

int main() {

}
```

多分上は無理なので

```cpp
　# include <iostream>
　# include <vector>
　# include <deque>
　# include <algorithm>
　# include <iomanip>
　# include <map>
　# include <memory>
　# include <cmath>
　# include <functional>
　# include <queue>
　# include <stack>
　# include <numeric>
　# include <string>
　using namespace std;
　#include <atcoder/all> //動くか怪しい
　using namespace atcoder;
　using mint = modint998244353;
　using ll   = long long;
　#define rep(i, n) for (ll i = 0; i < (n); i++)
　#define reps(i, a, b) for (ll i = (a); i < (b); i++)
　#define ft first
　#define sd second
　#define all(x) std::begin(x), std::end(x)

int main{

}
```

入出力

```cpp
    ofstream outputfile("test.txt");
    ifstream inputfile("A1");
    rep(i, n) inputfile >> arr[i];
    outputfile << arr[i]

    inputfile.close();
    outputfile.close();
```

# 忘れそうな書き方

queue

```cpp
    queue<int> q;
    q.push(10); //q.push_back(10)とかは普通の配列だね
    q.push(20);
    q.push(30);
    q.front();
    q.back();
    q.empty(); //qが空かどうか
    q.size() //qのサイズ
    q.pop() //最初の要素が消される
```

stack

```cpp
    stack<int> s;

    // 要素をスタックに追加
    s.push(10);
    s.push(20);
    s.push(30);

    s.top() // スタックの最上部の要素を表示

    s.pop();// 要素をスタックから取り出す

    s.empty()// スタックが空かどうかをチェック
```

```cpp
const auto &[h, c] = rectangles.back(); //これ便利

#include <bits/stdc++.h>
using namespace std;

int main() {
  vector<tuple<int, int, int>> a;
  a.push_back(make_tuple(3, 1, 1));
  a.push_back(make_tuple(1, 2, 100));
  a.push_back(make_tuple(3, 5, 1));
  a.push_back(make_tuple(1, 2, 3));
  sort(a.begin(), a.end());

  for (tuple<int, int, int> t : a) {
    int x, y, z;
    tie(x, y, z) = t;
    cout << x << " " << y << " " << z << endl;
  }
}


```

実行結果

```cpp
1 2 3
1 2 100
3 1 1
3 5 1
```

```cpp
 vec.pop_back(); // 末尾の要素を削除
//erase
 vector<int> vec = {1, 2, 3, 4};
vec.erase(vec.begin + 2); // {1, 2, 4}

vector<int> vec = {1, 2, 3, 4, 5, 6};
vec.erase(vec.begin(), vec.begin() + 2); // {4, 5, 6}
//remove　
vector<int> vec = {1, 0, 2, 0, 3, 0, 4, 0};
auto newEnd = remove(vec.begin(), vec.end(), 0);
for(auto it=vec.begin(); it != newEnd; ++it){
    cout << *it << " "; // 1 2 3 4
}
//unique関数　連続した重複する要素を削除する
vector<int> vec = {1, 2, 2, 2, 3, 4};
auto newEnd = unique(vec.begin(), vec.end());
for(auto it=vec.begin(); it != newEnd; ++it){
    cout << *it << " "; // 1 2 3 4
}

```

```cpp
 after[i] = make_tuple(x[i].ft + t, x[i].sd, rnum);
  ans += get<2>(after[i]) - newr;
```

```cpp
  set<int> s;
  s.insert(3);
  for( int x : s ) cout << x << " ";
    cout << endl;//各要素を列挙

   s.erase(7);


      //find
    decltype(s)::iterator it = s.find(3);
    int value;
    if(it != s.end()){
        value = *it;
        cout << value << endl;
    }else{
        cout << "not found" << endl;
    }

   it = s.find(8);
    if(it != s.end()){
        value = *it;
        cout << value << endl;
    }else{
        cout << "not found" << endl;
    }


      decltype(s)::iterator itl = lower_bound(s.begin(),s.end(),3);
    if(itl != s.end()){
        value = *itl;
        cout << value << endl;
    }else{
        cout << "not found" << endl;
    }

    decltype(s)::iterator itu = upper_bound(s.begin(),s.end(),3);
    if(itu != s.end()){
        value = *itu;
        cout << value << endl;
    }else{
        cout << "not found" << endl;
    }


```
