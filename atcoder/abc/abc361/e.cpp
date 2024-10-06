#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for (int i = 0; i < (n); ++i)
using ll = long long;

struct Edge {
  int to, cost;
  Edge(int to, int cost):to(to),cost(cost) {}
};

int main() {
  int n;
  cin >> n;
  ll ans = 0;
  vector<vector<Edge>> g(n);
  rep(i,n-1) {
    int a, b, c;
    cin >> a >> b >> c;
    a--; b--;
    g[a].emplace_back(b,c);
    g[b].emplace_back(a,c);
    ans += c*2;
  }

  auto dfs = [&](auto f, int v, ll d=0, int p=-1) -> pair<ll,int> {
    auto res = make_pair(d,v);
    for (auto e : g[v]) {
      if (e.to == p) continue;
      res = max(res, f(f, e.to, d+e.cost, v));
    }
    return res;
  };
  int a = dfs(dfs,0).second;
  ll diameter = dfs(dfs,a).first;
  ans -= diameter;
  cout << ans << endl;
  return 0;
}