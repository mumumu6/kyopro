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

    auto iskaibun = [&](string s) {
        string ss = s;
        reverse(all(ss));

        return s == ss;
    };

    vecc g(n, vec());
    vector<vector<char>> c(n, vector<char>(n));

    rep(i, n) rep(j, n) {
        char s;
        cin >> s;

        if (s != '-') {
            g[i].pb(j);
            c[i][j] = s;
        }
    }

    rep(i, n) rep(j, n) {
        queue<pair<ll, string>> q;
        q.push(mp(i, ""));

        while (!q.empty()) {
            auto [now, s] = q.front();
            q.pop();

            if (s.size() > 2 * n) {
                cout << -1 << " ";
                break;
            }

            if (now == j && iskaibun(s)) {
                cout << s.size() << " ";
                break;
            }

            for (auto next : g[now]) {
                q.push(mp(next, s + c[now][next]));
                // cout << s.size() << " " << c[now][next] << endl;
            }
        }
        if (j == n - 1) cout << endl;
    }
}