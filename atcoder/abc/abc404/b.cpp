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

    vector<vector<char>> s(n, vector<char>(n));
    vector<vector<char>> s1(n, vector<char>(n));
    vector<vector<char>> s2(n, vector<char>(n));
    vector<vector<char>> s3(n, vector<char>(n));
    vector<vector<char>> t(n, vector<char>(n));

    rep(i, n) rep(j, n) { cin >> s[i][j]; }
    rep(i, n) rep(j, n) {
        s3[i][j] = s[j][n - 1 - i];
        s2[i][j] = s[n - 1 - i][n - 1 - j];
        s1[i][j] = s[n - 1 - j][i];
    }
    rep(i, n) rep(j, n) cin >> t[i][j];

    vector<ll> cnt(4,0);

    rep(i, n) rep(j, n) {
        if (s[i][j] != t[i][j]) cnt[0]++;
        if (s1[i][j] != t[i][j]) cnt[1]++;
        if (s2[i][j] != t[i][j]) cnt[2]++;
        if (s3[i][j] != t[i][j]) cnt[3]++;

    //    cout << s1[i][j] << " ";
    //    if(j == n - 1) cout << endl;
    }

    ll rotate = 0;

    rep(i, 4) {
        if (cnt[i] + i < cnt[rotate] + rotate) rotate = i;

        // cout << cnt[i] << " " << rotate << endl;
    }

    cout << cnt[rotate] + rotate << endl;
}