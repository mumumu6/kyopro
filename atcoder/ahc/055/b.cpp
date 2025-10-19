#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld     = long double;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define x first
#define y second
#define all(x) std::begin(x), std::end(x)
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

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

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

template <class K, class V, class Comp, class Alloc>
ostream &operator<<(ostream &os, const map<K, V, Comp, Alloc> &m) {
    os << '{';
    for (auto &[k, v] : m) {
        if (k != m.begin()->first) os << ", ";
        os << k << ": " << v;
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
        if (id < sizeof...(Ts)) cerr << ", ";
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

    ll n; // n = 200
    cin >> n;

    vec h(n);
    vec c(n + 1);
    rep(i, n) cin >> h[i]; // 　宝箱の方さ
    rep(i, n) cin >> c[i]; // 耐久値
    c[n] = 4e10;           // 素手は最強

    // nを素手ということにする
    vecc a(n + 1, vec(n, 0));
    // 武器番号、 対象の宝箱、効率
    vector<vector<ld>> p(n + 1, vector<ld>(n));

    rep(i, n) rep(j, n) cin >> a[i][j];
    rep(i, n) a[n][i] = 1; // 素手の攻撃力は1にする
    rep(i, n) rep(j, n) {
        p[i][j] = (ld)h[j] / (ld)a[i][j]; // 割合的な物を持つ i番目の武器のj番目に対する効率
    }
    rep(i, n) p[n][i] = h[i]; // 素手の効率

    // 　箱番号、{効率、武器番号}
    vector<set<pair<ld, int>>> box_weapon_set(n);

    // pqでいくか

    rep(i, n) {
        box_weapon_set[i].insert({h[i], n});
    }
    set<int> broken_box; // 壊れた宝箱の集合

    rep(ni, n) {                     // n回宝箱を壊す
        set<pair<ll, int>> score; // 回数、箱
        rep(aim, n) {
            if (h[aim] <= 0) continue;
            ll cnt = 0;
            ll hp  = h[aim];
            // 壊れるまで
            for (auto [efi, weapon] : box_weapon_set[aim]) {
                if (hp <= 0) break;
                ll durability_now = (weapon == n) ? INF : c[weapon];
                if (durability_now <= 0) continue;

                ll dmg = (weapon == n) ? 1 : a[weapon][aim];
                if (dmg <= 0) continue;
                ll need = (hp + dmg - 1) / dmg;
                ll use  = min(durability_now, need);
                cnt += use;
                hp -= use * dmg;
            }
            if (hp > 0) cnt += hp;

            score.insert({cnt, aim});
        }
        if (score.empty()) break;

        auto [_, box] = *score.begin();

        while (h[box] > 0) {
            bool progressed = false;
            auto it         = box_weapon_set[box].begin();
            while (it != box_weapon_set[box].end()) {
                int weapon = it->second;
                if (c[weapon] <= 0) {
                    it = box_weapon_set[box].erase(it);
                    continue;
                }
                ll dmg = (weapon == n) ? 1 : a[weapon][box];
                if (dmg <= 0) {
                    ++it;
                    continue;
                }

                ll need = (h[box] + dmg - 1) / dmg;
                ll use  = (weapon == n) ? need : min<ll>(c[weapon], need);

                rep(i, use) {
                    int output_weapon = (weapon == n) ? -1 : weapon;
                    cout << output_weapon << " " << box << el;
                }

                h[box] -= use * dmg;
                if (weapon != n) c[weapon] -= use;
                progressed = true;
                if (h[box] <= 0) break;
                ++it;
            }
            if (!progressed) break;
        }

        if (h[box] > 0) {
            ll use = h[box];
            rep(i, use) { cout << -1 << " " << box << el; }
            h[box] = 0;
        }

        // 壊したら武器入手
        broken_box.insert(box);
        rep(i,n){
            if(broken_box.count(i)) continue;
            box_weapon_set[i].insert({p[box][i], box});
        }
    }
}
