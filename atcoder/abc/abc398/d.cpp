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
#define debug(x) cerr << #x << " = " << x << el

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, r, c;
    cin >> n >> r >> c;

    string s;
    cin >> s;

    pll fire   = {0, 0};
    pll player = {r, c};

    string ans = "";

    set<pll> issmoke;

    rep(i, n) {
        issmoke.insert({fire.ft, fire.sd});

        if (s[i] == 'N') {
            fire.ft++;
            player.ft++;
        }
        if (s[i] == 'S') {
            fire.ft--;
            player.ft--;
        }
        if (s[i] == 'E') {
            fire.sd--;
            player.sd--;
        }
        if (s[i] == 'W') {
            fire.sd++;
            player.sd++;
        }

        // cerr << fire.ft << " " << fire.sd << el;

        (issmoke.count({player.ft,player.sd})) ? ans += '1' : ans += '0';
    }

    cout << ans << el;
}