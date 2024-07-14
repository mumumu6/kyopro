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
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>


int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    int n;
    cin >> n;
    vec a(n);
    rep(i, n) cin >> a[i];

    vector<mint> ans(100, 0);
    ans[1] = n;
    ans[2] = n * (n - 1) / 2;

    queue<tuple<ll, ll, ll>> q;

    if(n==1){
        cout << n << " ";
        rep(i,n-1) cout << 0 << " ";
        return 0;
    }
    if(n==2){
        cout << n << " " << n*(n-1)/2 << " ";
        rep(i,n-2) cout << 0 << " ";
        return 0;
    }

    rep(i, n) reps(j, i + 1, n) {
        ll d = a[j] - a[i];
        q.push({j, 2, d});
        while (!q.empty() && j < n ) {
            auto [now, l, d] = q.front();
            q.pop();
            reps(k, now + 1, n) {
                if (a[k] == a[now] + d) {

                    q.push({k, l + 1, d});
                    ans[l + 1] += mint(1);
                }
            }
        }
    }

    rep(i, n) { cout << ans[i + 1].val() << " "; }
}