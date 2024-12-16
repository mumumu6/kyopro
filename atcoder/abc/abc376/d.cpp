#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a,b) make_pair(a,b)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(),z.end())
#define sor(z) sort(z.rbegin(),z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n,m;
    cin >> n >> m;

    vec a(m),b(m);

    vecc g(n,vec());

    rep(i,m) {
        cin >> a[i] >> b[i];
        a[i]--; b[i]--;

        g[a[i]].pb(b[i]);
    }


    queue<pll> q;
    vector<bool> visited(n,false);

    for(auto x : g[0]){
        q.push({x,1});
        visited[x] = true;
    }


    

    while(!q.empty()) {

        auto [x,cnt] = q.front();
        q.pop();
        if(x == 0){
            cout << cnt << endl;
            return 0;
        }

        for(auto y : g[x]){
            if(visited[y]) continue;

            q.push({y,cnt+1});
            visited[y] = true;
        }

        
    }

    cout << -1 << endl;
}