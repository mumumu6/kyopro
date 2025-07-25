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

    ll h, w;
    cin >> h >> w;

    vector<vector<ll>> a(h, vector<ll>(w));

    ll all_score = 0;

    rep(i, h) rep(j, w) {
        cin >> a[i][j];
        all_score ^= (a[i][j]);
    }

    vector<vector<bool>> used(h, vector<bool>(w, false));

    auto dfs = [&](auto dfs, vector<vector<bool>> used, ll score, int num) -> ll {
        ll n_ans = score;

        if (num == h * w) return n_ans;
        ll x = num / w, y = num % w;

        // 使わない場合
        chmax(n_ans, dfs(dfs, used, score, num + 1));

        if (used[x][y]) return n_ans; // すでに使われている場合はスキップ

        // 横使う場合

        ll nx = x + 1, ny = y;
        if (!(nx < 0 || nx >= h || ny < 0 || ny >= w)) {

            if (!used[nx][ny]) {

                vector<vector<bool>> tmp = used;

                tmp[x][y]    = true;
                tmp[nx][ny]  = true;
                ll new_score = score ^ a[x][y] ^ a[nx][ny];

                chmax(n_ans, dfs(dfs, tmp, new_score, num + 1));
            }
        }

        nx = x, ny = y + 1;
        if (!(nx < 0 || nx >= h || ny < 0 || ny >= w)) {

            if (!used[nx][ny]) {

                vector<vector<bool>> tmp = used;

                tmp[x][y]    = true;
                tmp[nx][ny]  = true;
                ll new_score = score ^ a[x][y] ^ a[nx][ny];

                chmax(n_ans, dfs(dfs, tmp, new_score, num + 1));
            }
        }

        return n_ans;
    };

    ll ans = dfs(dfs, used, all_score, 0);

    cout << ans << el;
}