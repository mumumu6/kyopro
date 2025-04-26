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

    ll n, m;
    cin >> n >> m;

    vector<pll> v(m);

    rep(i, m) { cin >> v[i].ft >> v[i].sd; }

    pll now = v[0];

    rep(i, m - 1) {
        ll x = v[i + 1].sd;
        ll y = v[i + 1].ft;

        ll dy = y - now.ft;
        ll dx = x - now.sd;

        // cerr << "dx : " << dx << " dy : " << dy << endl;

        if (abs(dx) >= 10 || min(abs(x - n + 1), abs(x)) < abs(dx)) {
            if (dx > 0) {
                cout << "S" << " " << "R" << endl;
                dx = x - n + 1;
            } else {
                cout << "S" << " " << "L" << endl;
                dx = x;
            }
        }

        rep(i, abs(dx)) {
            if (dx > 0) cout << "M" << " " << "R" << endl;
            else cout << "M" << " " << "L" << endl;
        }

        if (abs(dy) >= 10 || min(abs(y - n + 1), abs(y)) < abs(dy)) {

            if (dy > 0) {
                cout << "S" << " " << "D" << endl;
                dy = y - n + 1;
            } else {
                cout << "S" << " " << "U" << endl;
                dy = y - 0;
            }
            // cerr << "v[i + 1].ft : " << v[i + 1].ft << " dy : " << dy << endl;
        }

        rep(i, abs(dy)) {
            if (dy > 0) cout << "M" << " " << "D" << endl;
            else cout << "M" << " " << "U" << endl;
        }

        now = v[i + 1];
    }
}