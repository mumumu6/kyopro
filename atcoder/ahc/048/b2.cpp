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
    // 色を全部捨ててるので捨てるかすてないかを決める

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    auto squ = [](double x) { return x * x; };

    if (t > 19500 && d < 500) {
        rep(ii, n) {
            rep(i, n - 1) {
                if (i == 0) {
                    cout << 1 << " ";
                } else cout << 0 << " ";
            }
            cout << el;
        }
        rep(ii, n - 1) {
            rep(i, n) {
                if (ii == 0) {
                    cout << 1 << " ";
                } else {
                    cout << 0 << " ";
                }
            }
            cout << el;
        }

        int now_color_count = 0;
        int new_color_count = 0;
        Paint now_color(0, 0, 0, 0);
        rep(hi, h) {

            Paint &tr = target_color[hi];

            double error = 1010101;

            vector<pll> usecolor(3, {0, 0}); // 使う色の回数と絵具のidを持つ

            rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
                Paint &color1 = own_color[c1];
                Paint &color2 = own_color[c2];
                Paint &color3 = own_color[c3];

                rep(ca, 8) rep(cb, 8) rep(cc, 7) {
                    int cnt = 0;
                    if (ca) cnt++;
                    if (cb) cnt++;
                    if (cc) cnt++;

                    double sum = ca + cb + cc;
                    if (sum == 0) continue;

                    double r = (color1.red * ca + color2.red * cb + color3.red * cc) / sum;
                    double g = (color1.green * ca + color2.green * cb + color3.green * cc) / sum;
                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc) / sum;

                    double n_error =
                        sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 + cnt * d;

                    if (chmin(error, n_error)) {
                        usecolor[0].ft  = ca;
                        usecolor[1].ft  = cb;
                        usecolor[2].ft  = cc;
                        usecolor[0].sd  = c1;
                        usecolor[1].sd  = c2;
                        usecolor[2].sd  = c3;
                        new_color_count = cnt;
                        now_color.red   = r;
                        now_color.green = g;
                        now_color.blue  = b;
                        now_color.count = sum;
                        // debug(usecolor);
                    }
                }
            }
            sor(usecolor);

            // debug(hi, error);
            // debug(usecolor);
            // debug(now_color.red, now_color.green, now_color.blue, now_color.count);

            if (usecolor[1].ft != 0) {
                cout << 4 << spa << usecolor[0].ft << spa << 0 << spa << usecolor[0].ft + 1 << spa << 0
                     << el;
                cout << 1 << spa << 1 << spa << 0 << spa << usecolor[1].sd << el;
                if (usecolor[0].ft != usecolor[1].ft) {
                    cout << 4 << spa << usecolor[1].ft << spa << 0 << spa << usecolor[1].ft + 1 << spa
                         << 0 << el;
                    cout << 4 << spa << usecolor[0].ft << spa << 0 << spa << usecolor[0].ft + 1 << spa
                         << 0 << el;
                }
            }
            if (usecolor[2].ft != 0) {
                cout << 4 << spa << 0 << spa << usecolor[0].ft << spa << 0 << spa << usecolor[0].ft + 1
                     << el;
                cout << 1 << spa << 0 << spa << 1 << spa << usecolor[2].sd << el;
                if (usecolor[0].ft != usecolor[2].ft) {
                    cout << 4 << spa << 0 << spa << usecolor[2].ft << spa << 0 << spa
                         << usecolor[2].ft + 1 << el;
                    cout << 4 << spa << 0 << spa << usecolor[0].ft << spa << 0 << spa
                         << usecolor[0].ft + 1 << el;
                }
            }

            // vj
            if (usecolor[0].ft != 0) cout << 1 << spa << 0 << spa << 0 << spa << usecolor[0].sd << el;

            if (usecolor[1].ft != 0) cout << 4 << spa << 0 << spa << 0 << spa << 1 << spa << 0 << el;
            if (usecolor[2].ft != 0) cout << 4 << spa << 0 << spa << 0 << spa << 0 << spa << 1 << el;
            cout << 2 << spa << 0 << spa << 0 << el;

            if (usecolor[1].ft != 0)
                cout << 4 << spa << usecolor[1].ft << spa << 0 << spa << usecolor[1].ft + 1 << spa << 0
                     << el;

            if (usecolor[2].ft != 0)
                cout << 4 << spa << 0 << spa << usecolor[2].ft << spa << 0 << spa << usecolor[2].ft + 1
                     << el;

            rep(i, new_color_count) { cout << 3 << spa << 0 << spa << 0 << el; }

            if (usecolor[1].ft != 0) cout << 4 << spa << 0 << spa << 0 << spa << 1 << spa << 0 << el;
            if (usecolor[2].ft != 0) cout << 4 << spa << 0 << spa << 0 << spa << 0 << spa << 1 << el;
        }
    } else {
        rep(ii, n) {
            rep(i, n - 1) cout << 0 << " ";
            cout << el;
        }
        rep(ii, n - 1) {
            rep(i, n) cout << 0 << " ";
            cout << el;
        }

        ll c = (t / h) / 2;

        Paint now_color(0, 0, 0, 0);

        rep(hi, h) {

            Paint &tr = target_color[hi];

            double error = 1010101;

            ll usecount_a = 0;
            ll usecount_b = 0;
            ll usecount_c = 0;

            ll use_id1   = 0;
            ll use_id2   = 1;
            ll use_id3   = 1;
            ll dis_count = 0;

            Paint new_color = Paint(0, 0, 0, 0);

            ll qq = now_color.count / 4;
            ll rr = now_color.count % 4;
            rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
                Paint &color1 = own_color[c1];
                Paint &color2 = own_color[c2];
                Paint &color3 = own_color[c3];

                ll use_now_color = -qq;
                rep(dis, 4) {
                    use_now_color += (dis < rr ? qq + 1 : qq);

                    rep(ca, 8) rep(cb, 5) rep(cc, 3) {
                        if (ca + cb + cc > c) continue;

                        if (ca + cb + cc + use_now_color < 1) continue;

                        double sum = ca + cb + cc + use_now_color;

                        double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                    now_color.red * use_now_color) /
                                   sum;
                        double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                    now_color.green * use_now_color) /
                                   sum;
                        double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                    now_color.blue * use_now_color) /
                                   sum;

                        double n_error =
                            sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                            (ca + cb + cc) * d;

                        if (chmin(error, n_error)) {
                            usecount_a = ca;
                            usecount_b = cb;
                            usecount_c = cc;

                            use_id1         = c1;
                            use_id2         = c2;
                            use_id3         = c3;
                            new_color.red   = r;
                            new_color.green = g;
                            new_color.blue  = b;
                            new_color.count = sum;
                            dis_count       = now_color.count - use_now_color;
                        }
                    }
                }
            }
            now_color = new_color;

            // debug(error);

            // debug(now_color.red, now_color.green, now_color.blue, now_color.count);
            // debug(usecount_a, usecount_b, usecount_c, use_id1, use_id2, use_id3, dis_count);
            rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
            rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
            rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
            rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;

            cout << 2 << spa << 0 << spa << 0 << el;
            now_color.count--;
        }
    }
}