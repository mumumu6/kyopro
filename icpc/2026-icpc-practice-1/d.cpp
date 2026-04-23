#include <bits/stdc++.h>
using namespace std;
// #include <atcoder/all>
// using namespace atcoder;
// using mint = modint998244353;
using ll = long long;
using i128 = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define ft first
#define sd second
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

static inline bool inside(int x, int y, int w, int h)
{
    return 0 <= x && x < w && 0 <= y && y < h;
}

int main()
{
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    while (true)
    {
        ll m, n;
        
        set<pll> st;
        
        cin >> m;
        vector<pll> a(m); // 星座
        vector<pll> d(m); // 星座のずれ
        if (m == 0)
        break;
        rep(i, m)
        {
            cin >> a[i].ft >> a[i].sd;
        }

        // 星座のずれ
        d[0] = {0, 0};

        reps(i, 1, m)
        {
            d[i].ft = a[i].ft - a[0].ft;
            d[i].sd = a[i].sd - a[0].sd;
        }
        
        cin >> n;
        vector<pll> b(n); // 星

        rep(i, n)
        {
            cin >> b[i].ft >> b[i].sd;
            st.insert(b[i]);
        }

        rep(i, n)
        {
            bool flag = true;
            reps(j, 1, m)
            {
                ll x = b[i].ft + d[j].ft;
                ll y = b[i].sd + d[j].sd;
                if (!st.contains({x, y}))
                {
                    flag = false;
                    break;
                }
            }

            if (flag)
            {
                ll dxx = a[i].ft - b[i].ft;
                ll dyy = a[i].sd - b[i].sd;
                cout << dxx << " " << dyy << endl;
                break;
            }
        }
    }
}