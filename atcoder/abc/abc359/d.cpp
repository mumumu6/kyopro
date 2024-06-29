#include <bits/stdc++.h>
using namespace std;
#include "atcoder/all"
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second

#define all(x) std::begin(x), std::end(x)

ll n, k;
string s;

bool isOk(ll c, char a) {

    ll cc;
    if (a == 'A') cc = (c << 1) + 0;
    if (a == 'B') cc = (c << 1) + 1;
    // cerr << cc << " " << bitset<16>(cc).to_string().substr(16 - k) << endl;
    rep(i, k) {
        if (!(cc & (1 << i)) != !(cc & (1 << (k - i - 1)))) {
            return true;
        }
    }

    return false;
}

int main() {
    cin >> n >> k;
    cin >> s;

    vector<mint> dp(1 << (k - 1), 0);
    dp[0] = 1;

    rep(i, n) {
        vector<mint> ndp(1 << (k - 1), 0);

        rep(j, (1 << (k - 1))) {

            if (i < k - 1) {
                if (s[i] != 'B'){ 
                    ndp[((j << 1) + 0) % (1 << (k-1))] += dp[j];
                    //cout << 'A' << dp[j].va << " \n"[](j + 1 == 1 << (k - 1)); 
                    }

                if (s[i] != 'A'){
                     ndp[((j << 1) + 1) % (1 << (k-1))] += dp[j];
                     //cout << 'B' << dp[j] << " \n"[j + 1 == 1 << (k - 1)];
                }
                continue;
            }

            //cerr << i << " " << j << " "
            //     << bitset<16>(j).to_string().substr(16 - k) << endl;

            if (isOk(j, 'A') && s[i] != 'B') {
                ndp[((j << 1) + 0) % (1 << (k - 1))] += dp[j];
                //cout <<  j << "Bのほうだよ" << " " <<  dp[j].val()  << endl;
            }

            if (isOk(j, 'B') && s[i] != 'A') {
                ndp[((j << 1) + 1) % (1 << (k - 1))] += dp[j];
                //cout << j << "Aのほうだよ" << " " << dp[j].val() << endl;
            }

            cerr << isOk(j, 'A') << " " << isOk(j, 'B') << endl;
        }


        dp.swap(ndp);

        //rep(j, 1 << (k - 1)) {
        //    cerr << dp[j].val() << " \n"[j + 1 == 1 << (k - 1)];
        //}
    }

    mint sum = accumulate(all(dp), mint(0));
    cout << sum.val() << endl;
}