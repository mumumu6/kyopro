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
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
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

    ll n;
    cin >> n;
    vec a(n);
    vec b(n);
    rep(i, n) {
        cin >> a[i];
        b[i] = a[i] + i + 1;
    }

    sort(all(b));

    ll now = 0; // 成人で石が1つはある人の数
    ll itr = 0;

    vec c(n + 10,0);

    rep(i, n) {
        a[i] += now;
        c[min(a[i] + i,n+5)]++;
        
        a[i] -= n - i - 1;
        

        // cout << now << " " << a[i] << endl;

        if (a[i] < 0) a[i] = 0;
        now++; // 成人が一人増える

        // cout << "i: " << i << " itr: " << itr << " now: " << now << "a :" << a[i] << " b: " << b[itr]
        //      << endl;

        if (c[i] > 0) {
            now-= c[i]; ;
            // cout << "itr: " << itr << " now: " << now << "a :" << a[i] << " b: " << b[itr - 1] << endl;
        }
    }

    rep(i, n) cout << a[i] << " ";
    cout << endl;
}