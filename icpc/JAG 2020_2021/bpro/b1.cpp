#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using str = string;
using pint = pair<int, int>;
using pll = pair<ll, ll>;
template<class T>using vc = vector<T>;
template<class T>using vvc = vc<vc<T>>;
using vint = vc<int>;
using vll = vc<ll>;
using vstr = vc<str>;
using vpint = vc<pint>;
using vpll = vc<pll>;
using vbool = vc<bool>;
using vvint = vvc<int>;
using vvll = vvc<ll>;

#define overload4(_1, _2, _3, _4, name, ...) name
#define rep1(n)          for(ll i = 0; i < (n); ++i)
#define rep2(i, n)       for(ll i = 0; i < (n); ++i)
#define rep3(i, a, b)    for(ll i = (a); i < (b); ++i)
#define rep4(i, a, b, c) for(ll i = (a); i < (b); i += (c))
#define rep(...) overload4(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)
#define each1(a)       for(auto&& i : a)
#define each2(i,a)     for(auto&& i : a)
#define each3(x,y,a)   for(auto&& [x, y] : a)
#define each4(x,y,z,a) for(auto&& [x, y, z] : a)
#define each(...) overload4(__VA_ARGS__, each4, each3, each2, each1)(__VA_ARGS__)
#define all(x) (x).begin(), (x).end()

void yesno(bool flag){cout << (flag ? "Yes" : "No") << endl;}

bool in (ll x, ll y, ll w, ll h) { return 0 <= x && x < w && 0 <= y && y < h; }

void solve() {
    ll w, h, n, d, b; cin >> w >> h >> n >> d >> b;
    if (w == 0) exit(0);
    vll x(n), y(n);
    rep(n) {cin >> x[i] >> y[i]; x[i]--; y[i]--;}
    vvll field(h, vll(w, 0));
    rep(n) field[y[i]][x[i]] = 1;
    queue<pll> q; q.push({x[b-1], y[b-1]});
    vvll seen(h, vll(w, 0)); seen[y[b-1]][x[b-1]] = 1;
    ll ans = 1;
    while(!q.empty()){
        auto now = q.front(); q.pop();
        rep(d){
            vll dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
            rep(j,4){
                ll nx = now.first + (i+1)*dx[j], ny = now.second + (i+1)*dy[j];
                if (in(nx, ny, w, h) && field[ny][nx] == 1){
                    if (seen[ny][nx] == 0){
                        seen[ny][nx] = 1;
                        q.push({nx, ny});
                        ans++;
                    }
                }
            }
        }
    }
    cout << ans << endl;
}

int main(){
    rep(50) solve();
}