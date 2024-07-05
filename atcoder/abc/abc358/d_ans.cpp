#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (n); i++)
using namespace std;
using ll = long long;
int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    multiset<int> st;
    rep(i, n) st.insert(a[i]);
    ll ans = 0;
    rep(i, m) {
        auto v = st.lower_bound(b[i]);
        if (v == st.end()) {
            cout << -1 << '\n';
            return 0;
        }
        ans += *v;
        st.erase(v);
    }
    cout << ans << '\n';
}