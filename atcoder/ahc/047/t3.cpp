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

    vector<pair<ll, string>> v(n);

    string t;

    rep(i, n) { cin >> v[i].sd >> v[i].ft; }

    sort(all(v), greater<pair<ll, string>>());

    t = v[0].sd;

    set<char> st(all(t));
    for (char ch = 'a'; ch <= 'f' && t.size() < m; ch++) {
        if (!st.count(ch)) { t.push_back(ch); }
    }

    while (t.size() < m) t.push_back('a');

    // pos配列を二次元配列に変更
    vector<vector<int>> pos(6); // 各文字に対して複数のインデックスを保持

    rep(i, m) { pos[t[i] - 'a'].push_back(i); }

    // ── 遷移行列を組み立てる ──
    vector<vector<int>> A(m, vector<int>(m, 0));

    rep(i, m) A[i][(i + 1) % m] += 35;

    // ② 2 位・3 位の文字列をそれぞれ 30% で分岐
    for (int rank = 1; rank <= 1 && rank < n; ++rank) {
        const string &s = v[rank].sd; // ターゲット文字列
        if (s.size() < 2) continue;   // 安全チェック

        vec cnt(6, 0);
        rep(si, s.size()) cnt[s[si] - 'a']++;

        rep(si, s.size() - 1) {
            int from = s[si] - 'a';
            int to   = s[si + 1] - 'a';

            // fromの文字に対応する全てのインデックスに対して遷移を設定
            for (int from_idx : pos[from]) {
                // toの文字に対応する全てのインデックスに対して遷移を設定
                for (int to_idx : pos[to]) {
                    if (cnt[from] > 0) {
                        // 出現回数に応じて遷移確率を分配
                        A[from_idx][to_idx] += 62 / (cnt[from] * pos[from].size());
                    }
                }
            }
        }
    }

    // ③ 各行を 100% に整える（不足分を自ループ）
    rep(i, m) {
        int sm = 0;
        rep(j, m) sm += A[i][j];
        if (sm < 100) A[i][i] += 100 - sm;
    }

    // ── 出力 ──
    rep(i, m) {
        cout << t[i];
        rep(j, m) cout << ' ' << A[i][j];
        cout << el;
    }
    return 0;
}