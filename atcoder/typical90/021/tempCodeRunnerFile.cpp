#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
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

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    ofstream cout("test.txt");

    ll n, m;
    cin >> n >> m;
    vecc g(n), gg(n);
    vector<bool> used(n, false);
    rep(i, m) {
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        g[a].pb(b);
        gg[b].pb(a);
    }

    stack<ll> s, back;
    /*function<void(ll)> dfs1 = [&](ll u) {
        used[u] = true;
        for (auto x : g[u]) {
            if (!used[x]) dfs1(x);
        }
        back.push(u);
    };

    rep(i, n) {
        if (!used[i]) dfs1(i);
    }*/
    /*rep(i, n) {
        if (used[i]) continue;

        s.push(~i);
        s.push(i);
        while (!s.empty()) {
            ll x = s.top();
            s.pop();

            if (x >= 0) {
                used[x] = true;

                for (auto u : g[x]) {
                    if (!used[u]) {
                        s.push(~u);
                        s.push(u);
                    }
                }
            } else {
                back.push(~x);
            }
        }
    }*/
    int k = 0;
    vector<bool> done(n, false);
    rep(i, n) {
        if (used[i]) continue;
        s.push(~i);
        s.push(i);
        while (!s.empty()) {
            ll v = s.top();
            s.pop();

            if (v >= 0) {
                used[v] = true;
                for (auto u : g[v]) {
                    if (!used[u] && !done[u]) {
                        s.push(-u - 1);
                        s.push(u);
                        done[u] = true;
                        /*cout << " sの大きさ " << s.size() << " はこれ ";
                        stack<ll> ss = s;
                        while (!ss.empty()) {
                            cout << ss.top() << " ";
                            ss.pop();
                        }
                        cout << endl;*/
                    }
                }
            } else {
                back.push(-v - 1);
                // cout << "backに入れたのは " ;;
                // cout << ~v << " k " << k << endl;
                k++;
            }
        }
    }

    fill(all(used), false);
    ll ans                  = 0;
    ll cnt                  = 0;
    function<void(ll)> dfs2 = [&](ll pos) {
        used[pos] = true;
        cnt++;
        for (int i : gg[pos]) {
            if (used[i] == false) dfs2(i);
        }
    };
    multiset<ll> test;
    while (!back.empty()) {
        ll v = back.top();
        back.pop();
        cnt = 0;
        if (used[v]) continue;
        dfs2(v);
        test.insert(cnt);
        ans += cnt * (cnt - 1LL) / 2LL;
        // cout << cnt << " ";
    }
    cout << endl;
    /*rep(i, 1000) {
        if (test.count(i) == 0) continue;
        cout << i << " : " << test.count(i) << endl;
    }*/

    cout << ans << endl;

    /*while (!back.empty()) {

        ll v = back.top();
        back.pop();

        if (used[v]) continue;
        stack<ll> ss;
        ss.push(v);
        ll cnt = 1;
        while (!ss.empty()) {
            ll u = ss.top();
            ss.pop();
            if (!used[u]) {

                used[u] = true;
                for (auto x : gg[u]) {
                    if (!used[x]) {
                        cnt++;
                        ss.push(x);
                    }
                }
            }
        }

        ans += cnt * (cnt - 1LL) / 2LL;
    }

    cout << ans << endl;
    */
}