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

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k, Paint());
    vector<Paint> target_color(h, Paint());

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    auto squ = [](double x) { return x * x; };

    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;

    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {

        Paint &tr = target_color[hi];

        double error = 101010;

        ll usecount_a = 0;
        ll usecount_b = 0;

        ll use_id1 = 0;
        ll use_id2 = 1;

        Paint new_color = Paint(0, 0, 0, 0);

        while(now_color.count >= 3) {
            cout << 3 << spa << 0 << spa << 0 << el;
            now_color.count--;
        }

        rep(c1, k) reps(c2, c1 + 1, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];

            rep(ca, 5) rep(cb, 5) {
                if (ca + cb > c) continue;

                ll sum = ca + cb + now_color.count;

                double r = (color1.red * ca + color2.red * cb + now_color.red * now_color.count) / sum;
                double g =
                    (color1.green * ca + color2.green * cb + now_color.green * now_color.count) / sum;
                double b =
                    (color1.blue * ca + color2.blue * cb + now_color.blue * now_color.count) / sum;

                double n_error = squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue);

                if (chmin(error, n_error)) {
                    usecount_a = ca;
                    usecount_b = cb;

                    use_id1 = c1;
                    use_id2 = c2;
                    new_color.red = r;
                    new_color.green = g;
                    new_color.blue = b;
                    new_color.count = sum;
                }
            }
        }
        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;

        now_color = new_color;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
}