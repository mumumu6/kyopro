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

vector<int> generatePrimes(int limit) {
    std::vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false; // 0と1は素数ではない

    for (int i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // 素数リストを作成
    vector<int> primes;
    for (int i = 2; i <= limit; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);
    const int LIMIT = 3000000; // 素数を生成する上限
    vector<int> pri = generatePrimes(LIMIT);

    ll n;
    cin >> n;
    ll ans = 0;
    ll m   = round(sqrt(n));

    vector<ll> p;
    set<ll> prime = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

    ll t = 0;
    rep(i, 39) {
        if (!prime.count(i)) continue;
        p.pb(1);

        rep(j, 8) p[t] *= i;
        t++;
    }

    ans += upper_bound(all(p), n) - p.begin();


    rep(i, m) {
        if(!(binary_search(all(pri), i + 2))) continue;
        ll a = (i + 2) * (i + 2);
        ll b = floor(sqrt(n / a));
        if (i + 2 >= b) continue;
        ll  cnt = upper_bound(all(pri), b) - lower_bound(all(pri), i+2) - 1;
        ans += cnt;
    }

    cout << ans << endl;
}