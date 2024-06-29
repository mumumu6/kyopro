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

    int w, h, n, d, b;
    int count = 0;
    vector<vector<bool>> whbom;
    vector<vector<bool>> check;

    function<void(int, int)> bomb = [&](int a, int b) {
        count++;
        check[a][b] = true;
        reps(i, -d, d + 1) {
            if (i == 0) continue;
            if (0 <= a + i && a + i < w) {
                if (whbom[a + i][b] && !check[a + i][b]) {
                    bomb(a + i, b);
                }
            }
        }

        reps(i, -d, d + 1) {
            if (i == 0) continue;
            if (0 <= b + i && b + i < h) {
                if (whbom[a][b + i] && !check[a][b + i]) {
                    bomb(a, b + i);
                }
            }
        }
    };

    ofstream outputfile("ans.txt");

    while (cin >> w >> h >> n >> d >> b) {
        count = 0;
        whbom.assign(w, vector<bool>(h, false));
        check.assign(w, vector<bool>(h, false));

        int xf = -1, yf = -1;
        rep(i, n) {
            int a, b;
            cin >> a >> b;
            whbom[a - 1][b - 1] = true;
            if (i + 1 == b) {
                xf = a - 1;
                yf = b - 1;
            }
            cout << i << " ";
        }

        if (xf != -1 && yf != -1) {
            bomb(xf, yf);

            outputfile << count << endl;
        }

        outputfile.close();
        return 0;
    }
}