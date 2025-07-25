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

struct edge {
    ll to;   // 行き先
    ll cost; // 　コスト
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(8);

    const ll M = 1010101010;
    vector<bool> p(M, true);

    p[0] = p[1] = false;

    for (ll i = 0; i < M ; i++) {
        if(!p[i])continue;
        cout << i << endl;
        for (ll j = i* i; j < M; j += i) {
            p[j] = false;
        }
    }
}