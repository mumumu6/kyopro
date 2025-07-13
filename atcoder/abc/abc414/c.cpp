#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

// template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
//    return os << m.val();
// }

template <class T> ostream &operator<<(ostream &os, const vector<T> &v) {
    os << '[';
    for (ll i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    return os << ']';
}
template <class T> ostream &operator<<(ostream &os, const set<T> &s) {
    os << '{';
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it != s.begin()) os << ", ";
        os << *it;
    }
    return os << '}';
}

// ----------------- デバッグ用 -----------------
namespace dbg {
template <typename... Ts> void impl(const char *names, Ts &&...xs) {
    string_view sv{names};
    array<string_view, sizeof...(Ts)> labels{};
    ll pos = 0, idx = 0;
    for (ll i = 0; i <= sv.size(); ++i) {
        if (i == sv.size() || sv[i] == ',') {
            ll start = pos;
            while (start < i && sv[start] == ' ') ++start; // 前方空白除去
            labels[idx++] = sv.substr(start, i - start);
            pos           = i + 1;
        }
    }
    ll id    = 0;
    auto out = [&](auto &&v) {
        cerr << labels[id++] << " = " << v;
        if (id < sizeof...(Ts)) cerr << ",";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll a, n;
    cin >> a >> n;

    ll ans = 0;
    vector<string> s;
    ll cnt = 0;
    ll tmp = n;
    while (tmp > 0) {
        cnt++;
        tmp /= 10;
    }

    cnt = (cnt + 1) / 2;

    ll m = 1;
    rep(i, cnt) { m *= 10; }

    auto f = [&](string x) {
        ll i = 0, j = x.size() - 1;
        while (i < j) {
            if (x[i] != x[j]) return false;
            i++;
            j--;
        }
        return true;
    };

    reps(i, 1, m) {

        ll x = i;
        // i を ii にしてa進数に
        string t = "";
        ll tmp   = x;
        ll cnt   = 1;
        ll d     = 0;
        while (tmp > 0) {
            cnt *= 10;
            d++;
            tmp /= 10;
        }

        ll ii = x * cnt;

        tmp = x;

        rep(j, d) {
            ll a = tmp % 10;
            tmp /= 10;
            cnt /= 10;
            ii += a * cnt;
        }

        ll x1 = ii;

        if (ii <= n) {
            while (ii > 0) {
                t += to_string(ii % a);
                ii /= a;
            }

            if (f(t)) {
                ans += x1;
                // debug(ii, t, x1);
            }
        }

        // 12 を　121にする

        t   = "";
        tmp = i;
        cnt = 1;
        d   = 0;
        while (tmp > 0) {
            cnt *= 10;
            d++;
            tmp /= 10;
        }

        cnt /= 10;
        ll ii2 = i * (cnt);

        tmp = i / 10;
        rep(j, d - 1) {
            ll a = tmp % 10;
            tmp /= 10;
            cnt /= 10;
            ii2 += a * cnt;
        }

        ll x2 = ii2;

        if (ii2 > n) break;

        while (ii2 > 0) {
            t += to_string(ii2 % a);
            ii2 /= a;
        }

        if (f(t)) {
            ans += x2;
            // debug(ii2, t, x2);
        };
    }

    cout << ans << el;
    return 0;
}