#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
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
    cout << fixed << setprecision(20);

    int n;
    cin >> n;
    vec x(n);
    int sum = 0;
    rep(i, n) {
        cin >> x[i];
        sum += x[i];
    }

    ll ans1 = 0, ans2 = 0;
    ll p1 = sum / n;
    ll p2 = sum / n + 1;

    rep(i, n) {
        ans1 += (x[i] - p1) * (x[i] - p1);
        ans2 += (x[i] - p2) * (x[i] - p2);
    }

    cout << min(ans1, ans2) << endl;
}