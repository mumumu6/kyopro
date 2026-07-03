#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define rep(i, n) for (ll i = 0; i < (ll)(n); i++)
#define rep1(i, n) for (ll i = 1; i <= (ll)(n); i++)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; i--)
#define repbit(bit, n) for (ll bit = 0; bit < (1LL << (ll)(n)); bit++)
#define each(x, a) for (auto &x : a)
#define each2(a, b, c) for (auto &[a, b] : c)
#define yes_no(ans) cout << ((ans) ? "Yes" : "No") << endl;
#define YES_NO(ans) cout << ((ans) ? "YES" : "NO") << endl;
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()
#define sz(x) ((ll)(x).size())
#define nl "\n"
template <class T> bool chmax(T &a, const T &b) {
    if (a < b) {
        a = b;
        return true;
    }
    return false;
}
template <class T> bool chmin(T &a, const T &b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}
const string alpha = "abcdefghijklmnopqrstuvwxyz";
const string ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr ll INF   = (1LL << 60);
const ll dx[4]     = {1, 0, -1, 0};
const ll dy[4]     = {0, 1, 0, -1};
const double PI    = 3.141592653589793;
struct Edge {
    ll to;
    ll cost;
};
using Graph = vector<vector<ll>>;

int main() {
    while (true) {
        ll n, m;
        cin >> n >> m;
        if (n == 0 && m == 0) break;
        vector<ll> a(n), w(m);
        rep(i, n) { cin >> a[i]; }
        rep(i, m) { cin >> w[i]; }

        ll M = 1;
        rep(i, m) { M *= 3; }
        // ここ

        vector<set<ll>> sub(n);
        vector<bool> f(n, false);
        rep(i, M) {
            vector<ll> state(m);
            ll x = i;
            ll s = 0;
            rep(j, m) {
                ll t = x % 3;
                if (t == 1) s += w[j];
                else if (t == 2) s -= w[j];
                x /= 3;
            }

            if (s <= 0) continue;

            rep(j, n) {
                if (s == a[j]) {
                    f[j] = true;
                } else {
                    sub[j].insert(abs(s - a[j]));
                    sub[j].insert(abs(s + a[j]));
                }
            }
        }

        bool flag = true;
        ll cnt    = 0;
        ll start;
        rep(i, n) {
            if (!f[i]) {
                flag = false;

                if (cnt > 1) {
                    cnt++;
                    start = i;
                    break;
                }
            }
        }

        // //
        // for (auto x : sub[start]) {
        //     if (x <= 1e8) cout << x << nl;
        // }


        bool f1 = false;
        if (!flag) {
            if (cnt == 1) {
                cout << *sub[start].begin() << nl;
                f1 = true;
            }
            
            for (auto x : sub[start]) {
                // cout << x << nl;
                bool ff = true;
                for (ll j = start + 1; j < n; j++) {
                    if (f[j]) continue;
                    if (sub[j].find(x) == sub[j].end()) { ff = false; }
                }
                if (ff) {
                    cout << x << endl;
                    f1 = true;
                    break;
                }
                if (f1) { break; }
            }
        }

        if (flag && !f1) {
            cout << 0 << nl;
        } else if (!flag && !f1) {
            cout << -1 << nl;
        }
    }
}