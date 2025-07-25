#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
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
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'
// ----------------- 階上 ------------------
mint fact(ll n, bool inv = false) {
    if (n < 0) return 0; // 負数は0

    static vector<mint> factorial     = {1}; // pair<階乗, 階乗の逆数>
    static vector<mint> factorial_inv = {1}; // pair<階乗, 階乗の逆数>

    ll siz = factorial.size();

    if (n >= siz) {
        reps(i, siz, n + 1) factorial.emplace_back(factorial[i - 1] * i);
        factorial_inv.resize(n + 1);
        factorial_inv[n] = factorial[n].inv(); // ここだけ逆元をとる。
        for (ll i = n; i > siz; i--) factorial_inv[i - 1] = factorial_inv[i] * i;
    }

    return inv ? factorial_inv[n] : factorial[n];
}

mint perm(ll n, ll k) { return fact(n) * fact(n - k, true); }
mint comb(ll n, ll k) { return perm(n, k) * fact(k, true); }

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
    return os << m.val();
}

template <class T> ostream &operator<<(ostream &os, const vector<T> &v) {
    os << '[';
    for (ll i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    return os << ']';
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

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector own_color(k, vector<double>(3, 0));
    vector target_color(h, vector<double>(3, 0));

    rep(i, k) cin >> own_color[i][0] >> own_color[i][1] >> own_color[i][2];
    rep(i, h) cin >> target_color[i][0] >> target_color[i][1] >> target_color[i][2];

    ll c = (t / h) / 2;

    auto squ = [](double x) { return x * x; };

    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }

    double nor = 0;
    double nog = 0;
    double nob = 0;
    ll lim     = 0;

    rep(hi, h) {
        double &tr = target_color[hi][0];
        double &tg = target_color[hi][1];
        double &tb = target_color[hi][2];

        while (lim >= 10) {
            cout << 3 << spa << 0 << spa << 0 << el;
            lim--;
        }

        double error = 1010101010;

        ll usea = 0;
        ll useb = 0;
        ll usec = 0;
        ll used = 0;

        reps(ca, 1, 10) rep(cb, 10) rep(cc, 10) rep(cd, 10) {
            if (ca + cb + cc + cd > c) continue;
            if (ca + cb + cc + cd + lim < 1) continue;
            double r = (ca * own_color[0][0] + cb * own_color[1][0] + cc * own_color[2][0] +
                        cd * own_color[3][0] + nor * lim) /
                       (ca + cb + cc + cd + lim);
            double g = (ca * own_color[0][1] + cb * own_color[1][1] + cc * own_color[2][1] +
                        cd * own_color[3][1] + nog * lim) /
                       (ca + cb + cc + cd + lim);
            double b = (ca * own_color[0][2] + cb * own_color[1][2] + cc * own_color[2][2] +
                        cd * own_color[3][2] + nob * lim) /
                       (ca + cb + cc + cd + lim);

            if (chmin(error, (squ(r - tr) + squ(g - tg) + squ(b - tb)))) {
                usea = ca;
                useb = cb;
                usec = cc;
                used = cd;
                // debug(ca, cb, cc, cd, r, g, b, error);
            }
        }
        rep(i, usea) cout << 1 << spa << 0 << spa << 0 << spa << 0 << el;
        rep(i, useb) cout << 1 << spa << 0 << spa << 0 << spa << 1 << el;
        rep(i, usec) cout << 1 << spa << 0 << spa << 0 << spa << 2 << el;
        rep(i, used) cout << 1 << spa << 0 << spa << 0 << spa << 3 << el;

        lim += usea + useb + usec + used;
        nor = (usea * own_color[0][0] + useb * own_color[1][0] + usec * own_color[2][0] +
               used * own_color[3][0] + nor) /
              (lim);
        nog = (usea * own_color[0][1] + useb * own_color[1][1] + usec * own_color[2][1] +
               used * own_color[3][1] + nog) /
              (lim);
        nob = (usea * own_color[0][2] + useb * own_color[1][2] + usec * own_color[2][2] +
               used * own_color[3][2] + nob) /
              (lim);
        cout << 2 << spa << 0 << spa << 0 << el;
        lim--;
    }
}