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

ll overlap(const string &a, const string &b) {
    int lim = min(a.size(), b.size());
    for (int l = lim; l > 0; --l)
        if (a.substr(a.size() - l) == b.substr(0, l)) return l;
    return 0;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, m, l;
    cin >> n >> m >> l; // n = 36, m = 12,l = 1e6

    vector<pair<ll, string>> v(n);

    vector<char> c;

    string t;

    rep(i, n) { cin >> v[i].sd >> v[i].ft; }

    sort(all(v), greater<pair<ll, string>>());

    set<char> st;

    vector<pair<ll, string>> test(n); // スコア,文字列

    rep(ni, n - 1) {

        t = v[ni].sd;

        ll score = v[ni].ft;

        reps(i,ni + 1, n) {

            ll len1 = overlap(t, v[i].sd);
            ll len2 = overlap(v[i].sd, t);

            ll siz = v[i].sd.size();

            if (t.size() + siz - len1 > m) continue;
            if (t.size() + siz - len2 > m) continue;

            score += v[i].ft;

            if (len1 >= len2) {

                if (t.size() + siz - len1 <= m) { t += v[i].sd.substr(len1); }
            } else {
                if (t.size() + siz - len2 <= m) { t = v[i].sd.substr(0, siz - len2) + t; }
            }
        }

        test[ni] = {score, t};
    }

    sort(all(test), greater<pair<ll, string>>());
    t = test[0].sd;

    

    while (t.size() < m) { t += 'a'; }

    rep(i, m) {
        cout << t[i];
        for (int j = 0; j < m; ++j) {
            int p = (j == (i + 1) % m ? 100 : 0);
            cout << ' ' << p;
        }
        cout << '\n';
    }
}