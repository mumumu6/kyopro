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

    ll n, w;
    cin >> n >> w;

    ll t = w * 2;

    // 行が消滅する自国
    vec deleteTime;
    vecc blockRow = vecc(w, vec());

    vector<pair<ll, pll>> block(n); // 各ブロックの初期位置

    rep(i, n) {
        ll x, y;
        cin >> x >> y;
        x--;
        y--;

        block[i] = mp(i, mp(x, y));
        blockRow[x].pb(y);
    }

    rep(i, w) { so(blockRow[i]); }

    ll k = 0;
    while (true) {

        ll m      = 0;
        bool flag = true;
        rep(i, w) {
            if (blockRow[i].size() == k) {
                flag = false;
                break;
            }

            chmax(m, blockRow[i][k]);
        }
        if (!flag) break;
        deleteTime.pb(m + 1);
        k++;
    }

    ll q;
    cin >> q;

    rep(i, q) {
        ll t, a;
        cin >> t >> a;
        a--;

        auto [x, y] = block[a].sd;
        auto cnt    = lower_bound(all(blockRow[x]), y) - blockRow[x].begin();

        ll ny = y - t;
        if (ny >= cnt && ny > 0) {
            cout << "Yes" << endl;
        } else {
            auto it = upper_bound(all(deleteTime), t) - deleteTime.begin();
            
            if (deleteTime.size() != 0 && t < deleteTime[0]) it--;
            if (deleteTime.size() == 0) it = 0;
            // cout << "t:" << t << " deleteTime:" << deleteTime[0] << " it:" << it << endl;
            // 1,2,3,5,8

            // cout << "cnt:" << cnt <<  " it:" << it << " ny: " << ny << endl;

            if (ny >= cnt) {
                cout << "Yes" << endl;
            } else {
                if (cnt - it < 0) {
                    cout << "No" << endl;
                } else {
                    cout << "Yes" << endl;
                }
            }
        }
    }
}