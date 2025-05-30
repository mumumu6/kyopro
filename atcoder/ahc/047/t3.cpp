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

    ll n, m, l;
    cin >> n >> m >> l; // n = 36, m = 12,l = 1e6

    vector<pair<double, string>> v(n);

    string t = "";

    rep(i, n) { cin >> v[i].sd >> v[i].ft; }

    sort(all(v), greater<pair<double, string>>());

    vector<vector<double>> A(m, vector<double>(m, 1));

    rep(i, 12) t += 'a' + i / 2;

    rep(i, n) {
        const string &s = v[i].second;
        ll score        = pow(v[i].first / 1000, 1.4);
        rep(j, s.size() - 1) {
            int from = (s[j] - 'a') * 2;
            int to   = (s[j + 1] - 'a') * 2;

            int r = i % 2;
            // int q = (i + 1) % 2;
            // A[from][to] += 50 - r;
            if (i < 2) A[from + r][to + r] += score;
            // A[from][to + 1] += 50 - r;
            // A[from + 1][to + 1] += 50 - q;

            if (i == 2) A[from + r][to + r] += score / (4 + i / 2);
            // if (i == 3) A[from][to] += 30;
        }
    }

    A[(v[0].sd.back() - 'a') * 2][(v[1].sd[0] - 'a') * 2 + 1] += 40;
    A[(v[1].sd.back() - 'a') * 2 + 1][(v[0].sd[0] - 'a') * 2] += 40;

    // 正規化
    rep(i, m) {
        double sum = 1;
        rep(j, m) { sum += A[i][j]; }
        // debug(sum, i, A[i]);

        rep(j, m) {
            A[i][j] /= (double)sum / (double)100;
            A[i][j] = int(A[i][j]);
        }

        sum = 0;
        rep(j, m) sum += A[i][j];
        if (sum < 100) A[i][i] += 100 - sum;
    }

    // -- ここまで初期解　--

    //------------------------------------------------------------
    // 評価関数：once-or-more 近似（1 次連鎖 + ポアソン）
    //------------------------------------------------------------
    auto eval_poisson = [&](const vector<vector<double>> &w) -> long double {
        /* 1. 文字遷移確率行列 B[6][6] と文字ごとの状態数 cnt */
        double B[6][6] = {};
        int cnt[6]     = {};
        rep(i, m) cnt[t[i] - 'a']++;
        rep(i, m) rep(j, m) {
            int x = t[i] - 'a', y = t[j] - 'a';
            if (cnt[x]) B[x][y] += w[i][j] / 100.0 / cnt[x];
        }

        /* 2. 文字の定常分布 pi */
        vector<double> ps(m, 0.0), nxt(m);
        ps[0] = 1.0;    // 状態 0 からスタート
        rep(step, 40) { // 40 回で十分収束
            fill(all(nxt), 0.0);
            rep(i, m) rep(j, m) nxt[j] += ps[i] * w[i][j] / 100.0;
            swap(ps, nxt);
        }
        double pi[6] = {};
        rep(i, m) pi[t[i] - 'a'] += ps[i];

        /* 3. 各文字列 S_i の「1 回以上出る確率」Qi */
        long double total = 0;
        rep(idx, n) {
            const string &s = v[idx].sd;
            double q        = pi[s[0] - 'a']; // 先頭文字の確率
            rep(k, (int)s.size() - 1) {
                int a = s[k] - 'a', b = s[k + 1] - 'a';
                q *= B[a][b];
                if (q == 0) break; // あり得ない遷移
            }
            if (q == 0) continue;

            double lambda = max(0LL, l - (ll)s.size() + 1) * q; // 出現期待回数
            double Qi     = 1.0 - exp(-lambda);                 // ポアソン近似
            total += (long double)Qi * v[idx].ft;
        }
        return total; // ← long double
    };

    //------------------------------------------------------------
    // スコア → 対数スコア（焼きなまし用）
    //------------------------------------------------------------
    auto score_of = [&](const vector<vector<double>> &w) -> long double {
        return eval_poisson(w); // 変換・2 乗は不要
    };

    // 現在値（log をとるとき 0 回避に ε を足すと安全）
    long double best_log = logl(score_of(A) + 1e-12L);
    auto bestA           = A;

    //-------------------- 焼きなまし --------------------
    const double TIME_LIMIT = 1.9;
    const double T0 = 5e4, T1 = 1e2;
    auto start_clock = chrono::steady_clock::now();
    std::mt19937_64 rng(712367821);
    auto rand_int  = [&](int l, int r) { return uniform_int_distribution<int>(l, r)(rng); };
    auto rand_real = [&]() { return uniform_real_distribution<double>(0, 1)(rng); };

    while (true) {
        double t = chrono::duration<double>(chrono::steady_clock::now() - start_clock).count();
        if (t > TIME_LIMIT) break;
        double temp = T0 * pow(T1 / T0, t / TIME_LIMIT);

        /* ---- 近傍生成 ---- */
        int r = rand_int(0, m - 1);
        int u = rand_int(0, m - 1);
        int d = rand_int(0, m - 2);
        if (d >= u) ++d;
        int upLimit   = 100 - A[r][u];
        int downLimit = A[r][d];
        if (!upLimit || !downLimit) continue;
        int delta = rand_int(1, min(10, min(upLimit, downLimit)));

        A[r][u] += delta;
        A[r][d] -= delta;

        long double new_log = logl(score_of(A) + 1e-12L);
        long double diff    = new_log - best_log;

        if (diff > 0 || rand_real() < expl(diff / temp)) {
            if (diff > 0) {
                best_log = new_log;
                bestA    = A;
            }
        } else {
            A[r][u] -= delta;
            A[r][d] += delta;
        }
    }

    //------------------ ここまで焼きなまし ------------------

    // ── 出力 ──
    rep(i, m) {
        cout << t[i];
        rep(j, m) { cout << ' ' << (int)bestA[i][j]; }
        cout << el;
    }
    return 0;
}