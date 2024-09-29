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

    ll n, m, k;
    cin >> n >> m >> k;

    vec a(n);
    vec ans(n);
    rep(i, n) cin >> a[i];
    vec b = a;
    vec sum(n + 1, 0);

    sort(all(b));

    rep(i, n) sum[i + 1] = sum[i] + b[i];

    ll rest = k - sum[n];

    rep(i, n) {
        auto check = upper_bound(all(b), a[i] + rest) - b.begin();
        if (check <= n - m) {
            ans[i] = -1;
            continue;
        }

        auto f = [&](ll x) -> bool {
            ll now   = a[i] + x;
            auto itr = upper_bound(all(b), now) - b.begin();

            if (itr <= n - m) return false;

            ll lim = rest - x; // 他の候補者に入る票数
            if (lim < 0) return true;
            ll cnt = itr - n + m - 1; // 抜かされていい人数
            if (itr - cnt < 0) return true;
            ll ss = sum[itr] - sum[itr - cnt];

            auto whe = lower_bound(all(b), a[i]) - b.begin();

            ll t = 1;

            if (itr - cnt <= whe && itr - cnt - 1 >= 0 && whe < itr) {
                ss += b[itr - cnt - 1] - a[i];
                t++;
            } else if (itr - cnt <= whe) {
                return true;
            }

            ll d = lim - ((now + 1) * cnt - ss);

            // cout << "lim : " << lim << " itr : " << itr << ", whe : " << whe
            //  << " cnt: " << cnt << " d: " << d << " now :" << now << endl;

            if (d <= 0) return true;

            if (itr - cnt - t < 0) return true;


            if ((now - d - b[itr - cnt - t] < 0 && itr - cnt - t != whe ) ||
                (itr - cnt - t - 1 > 0 && now - d - b[itr - cnt - t - 1] < 0)) {
                return false;
            }

            return true;
        };

        ll ok = rest;
        ll ng = -1;
        // cout << "################################ i : " << i << ",  a[i] "
        //      << a[i] << endl;

        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;

            if (f(mid)) {
                ok = mid;
                // cout << "ok : " << ok << "  ng " << ng << endl;
            } else {
                ng = mid;
                // cout << "ok : " << ok << "  ng " << ng << endl;
            }
        }
        if (ok <= 0) ok = 0;
        ans[i] = ok;
    }

    rep(i, n) cout << ans[i] << " ";
    cout << endl;
}