#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
using i128 = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = { 0, 1, 1, 1, 0, INF, INF, INF }, cos45 = { 1, 1, 0, INF, INF, INF, 0, 1 };
vector<ll> dx = {0, 1, 0, INF};
vector<ll> dy = {1, 0,INF, 0};
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)
#define mp(a,b) make_pair(a,b)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(),z.end())
#define sor(z) sort(z.rbegin(),z.rend())
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
    ll id = 0;
    auto out  = [&](auto &&v) {
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

    while (true) {ll w, h;
    cin >> w >> h;
    if (w == 0 && h == 0) break;
    ll k = w + h - 1;
    vector<ll> x(k), y(k), n(k);
    vector<vector<ll>> v(w, vector<ll> (h, INF));
    rep(i, k) {
        cin >> x[i] >> y[i] >> n[i];
        x[i]--; y[i]--;
        v[x[i]][y[i]] = n[i];
    }

    vector<ll> H(h, INF);
    vector<ll> W(w, INF);
    H[0] = 0;

    stack<pair<ll, ll>> st;
    st.push({1, 0});
    while (!st.empty()) {
        ll d = st.top().first;
        ll num = st.top().second;
        st.pop();

        if (d < 0) {
            d *= (-1);
            d--;
            rep(i, h) {
                if (v[d][i] != INF && H[i] == INF) {
                    H[i] = v[d][i] - num;
                    st.push({(i+1), H[i]});
                }
                if (H[i] != INF && W[d] != INF) {
                    v[d][i] = H[i] + W[d];
                }
            }
        }
        else if (d > 0) {
            d--;
            rep(i, w) {
                if (v[i][d] != INF && W[i] == INF) {
                    W[i] = v[i][d] - num;
                    st.push({(i+1)*(-1), W[i]});
                }
                if (H[d] != INF && W[i] != INF) {
                    v[i][d] = H[d] + W[i];
                }
            }
        }
    }

    bool flag = true;
    rep(i, w) {
        if (W[i] == INF) {
            flag = false;
            break;
        }
    }
    rep(i, h) {
        if (flag && H[i] == INF) {
            flag = false;
            break;
        }
    }

    if (flag) cout << "YES" << endl;
    else cout << "NO" << endl;}

}