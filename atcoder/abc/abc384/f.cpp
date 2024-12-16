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

    vec pow(100, 0);
    pow[0]                 = 1;
    reps(i, 1, 100) pow[i] = pow[i - 1] * 2;

    ll n;
    cin >> n;
    ll ans = 0;
    vec a(n);
    vec d(n);
    vec cnt(100, 0);
    vec sum(n + 1, 0);
    vector<pair<ll, ll>> p(n); // 一つ目が2で割り切れる数,二つ目がその値
    rep(i, n) {
        cin >> a[i];
        p[i].sd = a[i];
        ll tmp  = a[i];
        while (tmp % 2 == 0) {
            tmp /= 2;
            d[i]++;
        }
        p[i].ft = d[i];
        cnt[d[i]]++;
    }
    sort(all(p));

    rep(i, n) sum[i + 1] = sum[i] + p[i].sd; // 累積和

    ll tmp = 0;
    ll now = 0;

    rep(i, n) {
        if (p[i].ft == tmp) now++;
        else {
            if (i == 0) {
                now = 1;
                tmp = p[i].ft;
                continue;
            };
            ans += now * (sum[n] - sum[i - 1]) / pow[p[i - 1].ft];
            ans+= n - i - 1; // 自分自身

            

            now = 1;
            tmp = p[i].ft;
        }
    }






    cout << ans << endl;
}