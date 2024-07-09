#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint1000000007;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<int>
#define vecc vector<vector<int>>

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n,m;
    cin >> n>>m;
    vecc g(n,vec());
    rep(i,m){
        int a,b;
        cin >> a >> b;
        a--;b--;
        g[a].pb(b);
        g[b].pb(a);
    }

    vector<int> dp(n,-1);
    vector<mint> cnt(n,0);
    queue<ll> q;
    dp[0] = 0;
    cnt[0] = 1;

    q.push(0);
    while(!q.empty()){
        ll v = q.front();
        q.pop();
        for(auto u: g[v]){
            if(dp[u] != -1){
                if(dp[u] == dp[v] + 1){
                    cnt[u] += cnt[v];
                }
                continue;
            }
            dp[u] = dp[v] + 1;
            q.push(u);
            cnt[u] += cnt[v];
        }



    }

    cout << cnt[n-1].val() << endl;
}