#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define ft first
#define sd second
#define all(x) std::begin(x), std::end(x)

int main() {
    ofstream outputfile("test.txt");

    ll n;
    cin >> n;
    vector<ll> arr(n);
    vector<ll> sum(n);
    rep(i, n) {
        cin >> arr[i];
        // outputfile << i << " ";
    }
    sum[0] = arr[0];
    sum[1] = arr[0] + arr[1];
    reps(i, 2, n) { sum[i] = arr[i] + arr[i - 1] + arr[i - 2]; }
    sort(all(sum));
    outputfile << sum[n - 1] << endl;

    return 0;
}
