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
        cout << labels[id++] << " = " << v;
        if (id < sizeof...(Ts)) cout << ", ";
    };
    (out(std::forward<Ts>(xs)), ...);
    cout << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

struct Paint {
    double red, green, blue;
    double count = 1;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0)
        : red(r), green(g), blue(b), count(c) {}
};

// 色の重み付き平均を計算する関数
Paint calculate_weighted_color_average(const vector<Paint> &colors, const vector<double> &weights,
                                       double base_amount = 0.0, const Paint &base_color = Paint()) {
    if (colors.size() != weights.size()) {
        debug("サイズ不一致でreturn Paint()");
        return Paint(); // エラーハンドリング
    }

    double total_weight = base_amount;
    double red_sum      = base_color.red * base_amount;
    double green_sum    = base_color.green * base_amount;
    double blue_sum     = base_color.blue * base_amount;

    for (int i = 0; i < colors.size(); i++) {
        if (weights[i] >= 0) {
            total_weight += weights[i];
            red_sum += colors[i].red * weights[i];
            green_sum += colors[i].green * weights[i];
            blue_sum += colors[i].blue * weights[i];
        }
    }

    if (total_weight == 0) { return Paint(0.0, 0.0, 0.0, 0.0); }

    // debug("total_weight:", total_weight);
    return Paint(red_sum / total_weight, green_sum / total_weight, blue_sum / total_weight,
                 total_weight);
}

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

    rep(ii, n) {
        rep(i, n - 1) {
            if ((i == 0 && ii > 1) || (ii == 1 && i == 1)) {
                cout << 1 << " ";
            } else cout << 0 << " ";
        }
        cout << el;
    }

    rep(ii, n - 1) {
        rep(i, n) {
            if ((ii == 0 && i > 1) || (ii == 1 && i == 1)) {
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
        ll dis_count = 0;

        Paint new_color = Paint(0, 0, 0, 0);

        int qq                  = (int)round(now_color.count) / 3;
        int rr                  = (int)round(now_color.count) % 3;
        double use_color_amount = 0;

        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];
            Paint &color3 = own_color[c3];

            ll use_now_color = -(0 < rr ? qq + 1 : qq);

            rep(dis, 3) {

                use_now_color += (dis < rr ? qq + 1 : qq);

                rep(ca, 6) rep(cb, 5) rep(cc, 3) {
                    int cnt = (ca != 0) + (cb != 0) + (cc != 0);

                    int maxc = max({ca, cb, cc});

                    double ra, rb, rc;
                    if (maxc != 0) {
                        // 割り算を一回だけ
                        double inv_maxc = 1.0 / (double)maxc;
                        ra              = ca * inv_maxc;
                        rb              = cb * inv_maxc;
                        rc              = cc * inv_maxc;
                    } else {
                        ra = 0.0, rb = 0.0, rc = 0.0;
                    }

                    double now_color_amount = use_now_color * (4 + ca + cb + cc - maxc) / 40.0;
                    double sum              = ra + rb + rc + now_color_amount;
                    if (sum == 0) continue;

                    double r = (color1.red * ra + color2.red * rb + color3.red * rc +
                                now_color.red * now_color_amount) /
                               sum;
                    double g = (color1.green * ra + color2.green * rb + color3.green * rc +
                                now_color.green * now_color_amount) /
                               sum;
                    double b = (color1.blue * ra + color2.blue * rb + color3.blue * rc +
                                now_color.blue * now_color_amount) /
                               sum;

                    double n_error =
                        sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 + cnt * d;

                    if (chmin(error, n_error)) {
                        usecolor[0].ft   = ca;
                        usecolor[1].ft   = cb;
                        usecolor[2].ft   = cc;
                        usecolor[0].sd   = c1;
                        usecolor[1].sd   = c2;
                        usecolor[2].sd   = c3;
                        use_color_amount = now_color_amount;
                        dis_count        = (ll)round(now_color.count) - use_now_color;

                        // debug(use_now_color, now_color_amount, ca, cb, cc);
                        // debug(r, g, b, sum, n_error);
                        // debug(color1.red, color2.red, color3.red, now_color.red);
                        // debug(ra, rb, rc, sum, now_color_amount);

                        // debug(now_color.red, now_color.green, now_color.blue, now_color.count);
                        // debug(use_now_color, cnt);
                    }
                }
            }
        }

        sor(usecolor);
        rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }

        if (usecolor[0].ft != 0) {
            cout << 4 << spa << 1 << spa << 0 << spa << 2 << spa << 0 << el;
            cout << 4 << spa << 0 << spa << 1 << spa << 0 << spa << 2 << el;
        }

        if (usecolor[1].ft != 0) {
            cout << 4 << spa << usecolor[0].ft + 1 << spa << 0 << spa << usecolor[0].ft + 2 << spa << 0
                 << el;
            cout << 1 << spa << 2 << spa << 0 << spa << usecolor[1].sd << el;
            if (usecolor[0].ft != usecolor[1].ft) {
                cout << 4 << spa << usecolor[1].ft + 1 << spa << 0 << spa << usecolor[1].ft + 2 << spa
                     << 0 << el;
                cout << 4 << spa << usecolor[0].ft + 1 << spa << 0 << spa << usecolor[0].ft + 2 << spa
                     << 0 << el;
            }
        }

        if (usecolor[2].ft != 0) {
            cout << 4 << spa << 0 << spa << usecolor[0].ft + 1 << spa << 0 << spa << usecolor[0].ft + 2
                 << el;
            cout << 1 << spa << 0 << spa << 2 << spa << usecolor[2].sd << el;
            if (usecolor[0].ft != usecolor[2].ft) {
                cout << 4 << spa << 0 << spa << usecolor[2].ft + 1 << spa << 0 << spa
                     << usecolor[2].ft + 2 << el;
                cout << 4 << spa << 0 << spa << usecolor[0].ft + 1 << spa << 0 << spa
                     << usecolor[0].ft + 2 << el;
            }
        }

        // 色の計算を関数化
        vector<Paint> mix_colors = {own_color[usecolor[0].sd], own_color[usecolor[1].sd],
                                    own_color[usecolor[2].sd], now_color};

        vector<double> mix_weights1 = {
            1.0, usecolor[1].ft != 0 ? (double)usecolor[1].ft / (double)usecolor[0].ft : 0.0,
            usecolor[2].ft != 0 ? (double)usecolor[2].ft / (double)usecolor[0].ft : 0.0,
            use_color_amount};

        Paint mixed_color1 = calculate_weighted_color_average(mix_colors, mix_weights1);

        vector<Paint> mix_colors2 = {own_color[usecolor[1].sd], own_color[usecolor[2].sd], now_color};

        vector<double> mix_weights2 = {
            usecolor[1].ft != 0 ? 1 - (double)usecolor[1].ft / (double)usecolor[0].ft : 0.0,
            usecolor[2].ft != 0 ? 1 - (double)usecolor[2].ft / (double)usecolor[0].ft : 0.0,
            now_color.count - dis_count - use_color_amount};

        // debug(1 - (double)usecolor[1].ft / (double)usecolor[0].ft);
        // debug(1 - (double)usecolor[2].ft / (double)usecolor[0].ft);
        // debug(mix_weights2);
        // debug(now_color.count - dis_count - use_color_amount);

        Paint mixed_color2 = calculate_weighted_color_average(mix_colors2, mix_weights2);

        Paint mixed = calculate_weighted_color_average(
            {mixed_color1, mixed_color2},
            {(double)(mixed_color1.count - 1), (double)(mixed_color2.count)});

        now_color       = mixed;
        now_color.count = round(mixed.count);

        // debug(mixed_color1.red, mixed_color1.green, mixed_color1.blue, mixed_color1.count);
        // debug(mixed_color2.red, mixed_color2.green, mixed_color2.blue, mixed_color2.count);
        // debug(mixed.red, mixed.green, mixed.blue, mixed.count);

        if (usecolor[1].ft != 0) cout << 4 << spa << 1 << spa << 0 << spa << 2 << spa << 0 << el;
        if (usecolor[2].ft != 0) cout << 4 << spa << 0 << spa << 1 << spa << 0 << spa << 2 << el;

        if (usecolor[0].ft != 0) cout << 1 << spa << 0 << spa << 0 << spa << usecolor[0].sd << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        if (usecolor[1].ft != 0)
            cout << 4 << spa << usecolor[1].ft + 1 << spa << 0 << spa << usecolor[1].ft + 2 << spa << 0
                 << el;

        if (usecolor[2].ft != 0) {
            cout << 4 << spa << 0 << spa << usecolor[2].ft + 1 << spa << 0 << spa << usecolor[2].ft + 2
                 << el;
        }

        if (usecolor[1].ft == 0 && usecolor[0].ft != 0)
            cout << 4 << spa << 1 << spa << 0 << spa << 2 << spa << 0 << el;
        if (usecolor[2].ft == 0 && usecolor[0].ft != 0)
            cout << 4 << spa << 0 << spa << 1 << spa << 0 << spa << 2 << el;
        // debug(hi, error);
        // debug(usecolor);
        // debug(now_color.red, now_color.green, now_color.blue, now_color.count);
        // for (auto &use : usecolor) {
        //     debug(own_color[use.sd].red, own_color[use.sd].green, own_color[use.sd].blue, use.ft);
        // }
        // debug(dis_count);
    }
}