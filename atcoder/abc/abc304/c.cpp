#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)

struct UnionFind{
    
    vector<int> par;

    void init(int sz){par.resize(sz,-1);}

    int root(int pos){
        if(par[pos] == -1)return pos;
        return par[pos] = root(par[pos]);
    }

    void unite(int u, int v){
        u = root(u);
        v = root(v);
        

        if(u==v)return;
        if(par[u] > par[v])swap(u,v);
        par[u] = v;
    }

    bool same(int u,int v){
        if(root(u) == root(v))return true;
        return false;
    }
};

UnionFind UF;

int dis(vector<int> a,vector<int> b){
    return (a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]);
}

int main() {
    int n,d;
    cin >> n >> d;
    UF.init(n);
    vector<vector<int>> p(n,vector<int>(2));

    rep(i,n)cin >> p[i][0] >> p[i][1];

    rep(i,n)reps(j,i+1,n){

        if(dis(p[i],p[j]) <= d*d){
            UF.unite(i,j);
        }

    }
    rep(i,n){
        if(UF.same(0,i)) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
}