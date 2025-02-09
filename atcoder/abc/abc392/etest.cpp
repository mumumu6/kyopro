#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;
using namespace atcoder;
using ll = long long;

class UFT{
private:
  int par[200001];
  int ranks[200001];
  int group;
  int N;

  void init(int n){
    for(int i = 0; i < n; i++){
      par[i] = i;
      ranks[i] = 0;
    }
    group = n;
    N = n;
  }
public:
  UFT(int N){
    init(N);
  }

  int find(int x){
    if(par[x] == x) return x;
    else return find(par[x]);
  }

  void unite(int x, int y){
    x = find(x);
    y = find(y);
    if(x == y)return;

    if(ranks[x] < ranks[y]) par[x] = y;
    else if(ranks[x] == ranks[y]){
      par[y] = x;
      ranks[x]++;
    }
    else par[y] = x;
    group--;
  }

  bool same(int x, int y){
    return find(x) == find(y);
  }

  int count_group(){
    return group;
  }

  map<int, int> count_elements(){
    map<int, int>mp;
    for(int i = 0; i < N; i++)mp[find(i)]++;

    return mp;
  }
  
  set<int>enumerate_leaders(){
	  set<int> res;
	  for(int i = 0; i < N; i++)res.insert(find(i));
	  return res;
  }
};

// --- あなたのコードの関数 ---
vector<string> your_code(int N, int M, vector<pair<int, int>> edges) {
    dsu uf(N);
    vector<string> result;
    vector<pair<int, int>> extra;

    for (int i = 0; i < M; i++) {
        auto [a, b] = edges[i];
        a--, b--;
        if (uf.same(a, b)) extra.push_back({a, i + 1});
        else uf.merge(a, b);
    }

    auto g = uf.groups();
    result.push_back(to_string(g.size() - 1));
    set<ll> merged;
    ll t = 0;
    ll mergedLeader;

    for (size_t i = 0; i < extra.size(); i++) {
        if (i == 0) {
            mergedLeader = uf.leader(extra[0].first);
            merged.insert(mergedLeader);
        }
        auto [a, num] = extra[i];
        ll la         = uf.leader(a);

        if (merged.count(la)) {
            while (t < g.size() && merged.count(uf.leader(g[t][0]))) t++;
            if (t >= g.size()) continue;
            result.push_back(to_string(num) + " " + to_string(la + 1) + " " +
                             to_string(uf.leader(g[t][0]) + 1));
        } else {
            result.push_back(to_string(num) + " " + to_string(la + 1) + " " +
                             to_string(mergedLeader + 1));
            merged.insert(la);
        }
        if (i >= g.size() - 2) break;
    }
    return result;
}

// --- 正しいとされるコードの関数 ---
vector<string> correct_code(int N, int M, vector<pair<int, int>> edges) {
    UFT tree(N);
    vector<string> result;
    vector<int> ex;
    for (int i = 0; i < M; i++) {
        auto [A, B] = edges[i];
        A--, B--;
        if (tree.same(A, B)) ex.push_back(i);
        tree.unite(A, B);
    }
    int cnt = tree.count_group() - 1;
    result.push_back(to_string(cnt));
    set<int> leaders = tree.enumerate_leaders();
    for (int i = 0; cnt > 0 && i < (int)ex.size(); i++) {
        result.push_back(to_string(ex[i] + 1) + " " + to_string(edges[ex[i]].second + 1) + " " +
                         to_string(*leaders.begin() + 1));
        leaders.erase(*leaders.begin());
        cnt--;
    }
    return result;
}

int main() {
    int N, M;
    N = 100000;
    M = 300;

    vector<pair<int, int>> edges(M);
    for (int i = 0; i < M; i++) {
        int a = rand() % N + 1;
        int b;
        do { b = rand() % N + 1; } while (a == b);
        edges[i].first  = a;
        edges[i].second = b;
    }

    vector<string> res1 = your_code(N, M, edges);
    vector<string> res2 = correct_code(N, M, edges);

    if (res1 != res2) {
        cout << "Error: Outputs do not match!" << endl;
        cout << "Your Output:" << endl;
        for (const auto &line : res1) cout << line << endl;
        cout << "Correct Output:" << endl;
        for (const auto &line : res2) cout << line << endl;
    } else {
        cout << "Outputs match!" << endl;
    }
    return 0;
}
