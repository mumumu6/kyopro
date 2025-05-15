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

    vector<ll> a(n);

    rep(i, n) { cin >> a[i]; }
    sort(a.rbegin(), a.rend());
    vector<ll> b(a[0]);

    ll now = 0;
    rep(i, n) {
        ll j = n - 1 - i;

        rep(k, a[j] - now) {
            b[now + k] = j + 1;
            // cout << now  +  k << " " << j + 1 << endl;
        } // これでグラフを横から見る
        now = a[j];
    }

    // for (auto x : b) {
    //     cout << x << " ";
    //     cout << endl;
    // }

    pll used = {-1, 0}; // ここまで使ったポート

    rep(i, n) {
        auto [nowx, nowy] = used;

        // あと何個残ってるかを計算
        ll les = a[i] - nowy;

        if (nowx >= i) les--;
        if (nowx + 1 == i) nowx++;
        cerr << " i = " << i << " les = " << les << " nowx = " << nowx << " nowy = " << nowy
             << " b[nowy] = " << b[nowy] << endl;

        if (les == 0) continue;
        if (nowx + les <= b[nowy] - 1) {
            used = {nowx + les, nowy};
            cerr << "used = " << used.first << " " << used.second << endl;
        } else {
            ll over = nowx + les - b[nowy] + 1;
            cerr << "over = " << over << " b[nowy] = " << b[nowy] << " nowy = " << nowy << endl;

            if (over + i <= nowx && over + i <= b[nowy] - 1 ) {
                used = {over + i, nowy + 1};
                cerr << "used = " << used.first << " " << used.second << endl;
            } else {
                cout << "No" << endl;
                return 0;
            }
        }
    }

    cout << "Yes" << endl;
}