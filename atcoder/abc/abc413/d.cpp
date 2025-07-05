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

    ll t;
    cin >> t;

    rep(ti, t) {
        ll n;
        cin >> n;
        vector<ll> a;
        vector<ll> b;
        set<ll> s;

        rep(i, n) {
            ll x;

            cin >> x;
            if (x > 0) a.pb(x); // 0はない
            else b.pb(x);

            s.insert(x);
        }
        sort(all(a));
        sort(all(b), greater<ll>());

        if (s.size() == 1) { // 比が1ならok
            Yes;
            // debug("all same");
            continue;
        }

        bool ok = true;
        // ll as   = a.size();
        // ll bs   = b.size();
        // if (bs > 0 && as > 0 && std::abs(as - bs) > 1) {
        //     No;
        //     continue;
        // }
        if (b.size() == 0) { // 正の数のみ
            rep(i, n - 2) {

                if (a[i] * a[i + 2] != a[i + 1] * a[i + 1]) {
                    ok = false;
                    // debug(a[i], a[i + 1], a[i + 2]);
                    break;
                }
            }
        } else {
            vec c;
            // a[0]

            ll as = a.size();
            ll bs = b.size();
            if (std::abs(as - bs) > 1) {
                No;
                continue;
            }

            if(a.size() == 0){
                No;
                continue;
            }

            if (abs(a[0]) > abs(b[0])) { // aのほうが絶対値が小さい
                swap(a, b);
            }

            if (a.size() < b.size() || a.size() - b.size() > 1) { // aのほうが小さい場合はbの最後を追加
                No; // a開始なのにbのほうが数が大きかったらむり
                continue;
            }

            rep(i, b.size()) {
                c.pb(a[i]);
                c.pb(b[i]);
            }
            if (a.size() > b.size()) c.pb(a.back()); // aのほうが大きい場合は最後にaを追加

            rep(i, n - 2) {

                if (c[i] * c[i + 2] != c[i + 1] * c[i + 1]) {
                    ok = false;
                    // debug(a[i], a[i + 1], a[i + 2]);
                    break;
                }
            }
        }   

        // debug(a, b, ok);
        if (ok) Yes;
        else No;
    }
}