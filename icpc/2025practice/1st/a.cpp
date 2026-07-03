#include <bits/stdc++.h>
using namespace std;
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

    ll m, n;
    cin >> m >> n;

    vector<string> s(m);

    rep(i, m)  cin >> s[i];

    ll cost = 0;

    vector<ll> sum1(n + 1, 0); // 縦の個数
    vector<ll> sum2(m + 1, 0); // 横

    rep(i, n) {
        ll cnt = 0;
        rep(j, m) {
            if (s[j][i] == 'o') cnt++;
        }
        // cerr << cnt << " ";
        sum1[i + 1] = sum1[i] + cnt;
    }

    rep(j, m) {
        ll cnt = 0;
        rep(i, n) {
            if (s[j][i] == 'o') cnt++;
        }

        sum2[j + 1] = sum2[j] + cnt;
    }

    // rep(i,n + 1) cout << sum1[i] << " ";
    // cout << endl;
    // rep(i,m + 1) cout << sum2[i] << " ";
    // cout << endl;


    ll ans = 4e18;
    // 上がバツ領域

    reps(i, 1, n) {
        // 上側の丸の個数
        ll up   = sum1[i];
        ll down = (n - i) * m - (sum1[n] - sum1[i]); // 下側のバツの個数
        chmin(ans, up + down);
    }

    reps(i, 1, n) {
        ll up   = i * m - sum1[i];                   // 上側のバツの個数
        ll down =  (sum1[n] - sum1[i]); // 下側のまる

        chmin(ans, up + down);
    }

    // 右がばつ領域

    reps(i, 1, m) {
        ll left  = sum2[i];                           // 左側のまる
        ll right = (m - i) * n - (sum2[m] - sum2[i]); // 右側のばつ

        chmin(ans, left + right);

        // cout << "left = " << left << " right" << right << endl;
    }

    // 左がばつ

    // cout << "------------" << endl;

    reps(i, 1, m) {
        ll left  = i * n - sum2[i];   // 左側のばつ
        ll right = sum2[m] - sum2[i]; // 右側の丸

        chmin(ans, left + right);
        // cout << "left = " << left << " right" << right << endl;
    }

    cout << ans << endl;
}