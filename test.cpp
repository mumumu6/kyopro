#include <bits/stdc++.h>
using namespace std;
using ll     = long long;
const ll INF = 4e18;
using i128 = __int128_t;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)


long long modpow(long long a, long long n, long long mod) {
    long long res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = (i128)res * a % mod;
        a = (i128)a * a % mod;
        n >>= 1;
    }
    return res;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n == 2) return true; // 2と3は素数
    if (n % 2 == 0) return false;

    vector<ll> bases = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    ll d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (auto a : bases) {
        if (a % n == 0) continue;
        a %= n;
        ll x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = true; // 合成数だったらok

        reps(r, 1, s) {


            x = (i128)(x)*x % n;
            if (x == n - 1) {
                ok = false;
                break;
            }
        }
        if (ok) return false; // 合成数
    }

    return true;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll q;
    cin >> q;

    rep(qi, q) {
        ll n;
        cin >> n;

        if (is_prime(n)) {
            cout << "Yes" << "\n";
        } else {
            cout << "No" << "\n";
        }
    }
}