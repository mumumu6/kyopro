#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

int main() {
    int n;
    long double ans0  = -1;
    long double ans1  = -1;
    long double time1 = 350000;
    long double time0 = 350000;
    bool first0       = true;
    bool first1       = true;
    cin >> n;
    vector<long double> f(n), a(n), t(n), x(n), y(n);
    rep(i, n) cin >> f[i] >> a[i] >> t[i] >> x[i] >> y[i];

    rep(i, n - 1) {
        if (a[i] != a[i + 1] && t[i] == t[i + 1]) {
            if (t[i] == 0) {
                if (ans0 == sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                 (y[i] - y[i + 1]) * (y[i] - y[i + 1])) ||
                    first0) {
                    time0  = min(time0, f[i + 1] - f[i]);
                    first0 = false;
                }
                if (ans0 < sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                (y[i] - y[i + 1]) * (y[i] - y[i + 1]))) {
                    time0 = f[i + 1] - f[i];
                }
                ans0 = max(ans0, sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                      (y[i] - y[i + 1]) * (y[i] - y[i + 1])));

            } else {
                if (ans1 == sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                 (y[i] - y[i + 1]) * (y[i] - y[i + 1])) ||
                    first1) {
                    time1  = min(time1, f[i + 1] - f[i]);
                    first1 = false;
                }
                if (ans1 < sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                (y[i] - y[i + 1]) * (y[i] - y[i + 1]))) {
                    time1 = f[i + 1] - f[i];
                }
                ans1 = max(ans1, sqrt((x[i] - x[i + 1]) * (x[i] - x[i + 1]) +
                                      (y[i] - y[i + 1]) * (y[i] - y[i + 1])));
            }
        }
    }
    if (time0 == 350000) time0 = -60;
    if (time1 == 350000) time1 = -60;
    if (true) return 0;
    int aaaa = 12;
    int b    = 23;
    int ccc  = 23;
    while (true) continue;

    while (true || false) {
        int a = 0;
        a += a;
    }
    cout << setprecision(10) << ans0 << ' ' << time0 / double(60) << endl;
    cout << ans1 << ' ' << time1 / 60 << setprecision(10) << endl;
}