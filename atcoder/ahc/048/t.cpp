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
#define spa " "
#define el '\n'

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

struct Color {
    double r, g, b;
    Color(double r = 0, double g = 0, double b = 0) : r(r), g(g), b(b) {}
    Color operator*(double x) const { return Color(r * x, g * x, b * x); }
    Color operator+(const Color &o) const { return Color(r + o.r, g + o.g, b + o.b); }
    Color operator-(const Color &o) const { return Color(r - o.r, g - o.g, b - o.b); }
    double norm() const { return sqrt(r * r + g * g + b * b); }
};

double det3(Color a, Color b, Color c) {
    return a.r * (b.g * c.b - b.b * c.g) - a.g * (b.r * c.b - b.b * c.r) + a.b * (b.r * c.g - b.g * c.r);
}

bool solve_barycentric(const Color &A, const Color &B, const Color &C, const Color &T, double &x,
                       double &y, double &z) {
    double D = det3(A, B, C);
    if (abs(D) < 1e-8) return false;
    x = det3(T, B, C) / D;
    y = det3(A, T, C) / D;
    z = det3(A, B, T) / D;
    return true;
}

tuple<int, int, int> find_best_integer_mix(const Color &A, const Color &B, const Color &C,
                                           const Color &T, int max_total) {
    double x, y, z;
    if (!solve_barycentric(A, B, C, T, x, y, z)) return {1, 0, 0};
    double total = x + y + z;
    x *= max_total / total;
    y *= max_total / total;
    z *= max_total / total;

    int best_a = 0, best_b = 0, best_c = 0;
    double best_error = 1e9;
    for (int da = -1; da <= 1; da++) {
        for (int db = -1; db <= 1; db++) {
            for (int dc = -1; dc <= 1; dc++) {
                int ca  = max(0, (int)round(x) + da);
                int cb  = max(0, (int)round(y) + db);
                int cc  = max(0, (int)round(z) + dc);
                int sum = ca + cb + cc;
                if (sum == 0 || sum > max_total) continue;
                Color mix    = (A * ca + B * cb + C * cc) * (1.0 / sum);
                double error = (mix - T).norm();
                if (error < best_error) {
                    best_error = error;
                    best_a = ca, best_b = cb, best_c = cc;
                }
            }
        }
    }
    return {best_a, best_b, best_c};
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k);
    vector<Paint> target_color(h);

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    rep(ii, n) {
        rep(i, n - 1) cout << 0 << spa;
        cout << el;
    }
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << spa;
        cout << el;
    }

    ll c = (t / h) / 2;
    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        Paint &tr = target_color[hi];

        double error = 101010;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1;
        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;

        Paint new_color = Paint(0, 0, 0, 0);
        while (now_color.count >= 1) {
            cout << 3 << spa << 0 << spa << 0 << el;
            now_color.count--;
        }

        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            Color A = {own_color[c1].red, own_color[c1].green, own_color[c1].blue};
            Color B = {own_color[c2].red, own_color[c2].green, own_color[c2].blue};
            Color C = {own_color[c3].red, own_color[c3].green, own_color[c3].blue};
            Color T = {tr.red, tr.green, tr.blue};

            auto [ca, cb, cc] = find_best_integer_mix(A, B, C, T, c);
            if (ca + cb + cc == 0) continue;
            double sum     = ca + cb + cc;
            Color mix      = (A * ca + B * cb + C * cc) * (1.0 / sum);
            double n_error = (mix - T).norm() * 10000 + (ca + cb + cc) * 2 * d;

            if (chmin(error, n_error)) {
                usecount_a      = ca;
                usecount_b      = cb;   
                usecount_c      = cc;
                use_id1         = c1;
                use_id2         = c2;
                use_id3         = c3;
                new_color.red   = mix.r;
                new_color.green = mix.g;
                new_color.blue  = mix.b;
                new_color.count = sum;
            }
        }

        rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
        rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
        rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;
        now_color = new_color;
        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }
}
