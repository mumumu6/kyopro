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

int w, h, n, d, b;
int cnt = 0;
vector<vector<bool>> whbom;
vector<vector<bool>> check;

void bomb(int a, int b) {
    // cout << 'a' << a << 'b' << b << endl;
    cnt++;
    check[a][b] = true;
    reps(i, -d, d + 1) {
        if (!(0 <= a + i && a + i < w)) continue;
        if (whbom[a + i][b] && !check[a + i][b]) {
            bomb(a + i, b);
            // cout << "爆弾は" << a << " " << b << endl;
        }
        // cout << i << " ";
    }

    reps(i, -d, d + 1) {
        if (!(0 <= b + i && b + i < h)) continue;
        if (whbom[a][b + i] && !check[a][b + i]) {
            // cout << "爆弾は" << a << " " << b << endl;
            bomb(a, b + i);
        }
    }
}

int main() {

    ofstream outputfile("ans.txt");
    bool kaigyo = false;
    while (  cin >> w >> h >> n >> d >> b && w!= 0) {
        cnt = 0;
       
        whbom.assign(w, vector<bool>(h, false));
        check.assign(w, vector<bool>(h, false));
        int xf = -1, yf = -1;

        rep(i, n) {
            int x, y;
            cin >> x >> y;
            whbom[x - 1][y - 1] = true;
            if (i + 1 == b) {
                xf = x - 1;
                yf = y - 1;
            }
        }
        if (xf != -1 && yf != -1) bomb(xf, yf);
        if (kaigyo) outputfile << endl;
        kaigyo = true;
        outputfile << cnt ;


    }

     outputfile.close();
    return 0;
}