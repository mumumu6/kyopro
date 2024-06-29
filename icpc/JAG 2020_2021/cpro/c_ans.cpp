#include <bits/stdc++.h>
using namespace std;
#define repeat(i, n) for (int i = 0; i < (n); ++i)

const int MAX_SIZE = 1001;
// [1, MAX_SIZE), [1, MAX_SIZE)の盤面を扱う
int Left[MAX_SIZE + 10][MAX_SIZE + 10];
int Up[MAX_SIZE + 10][MAX_SIZE + 10];
int dp[MAX_SIZE + 10][MAX_SIZE + 10];

const int INF = 1e9;

int main() {
    int N;
    while (cin >> N, N) {
        memset(Left, 0, sizeof(Left));
        memset(Up, 0, sizeof(Up));
        repeat(i, MAX_SIZE) repeat(j, MAX_SIZE) dp[i][j] = INF;
        int sx, sy, gx, gy;
        cin >> sx >> sy >> gx >> gy;
        vector<int> X(N), Y(N), R(N);
        repeat(i, N) cin >> X[i] >> Y[i] >> R[i];

        if (sx > gx) {
            // xを反転
            // (x, y) -> (MAX_SIZE - x, y)
            sx = MAX_SIZE - sx;
            gx = MAX_SIZE - gx;
            assert(sx <= gx);
            assert(1 <= sx and sx < MAX_SIZE);
            assert(1 <= gx and gx < MAX_SIZE);
            repeat(i, N) {
                X[i] = MAX_SIZE - X[i];
            }
        }

        if (sy > gy) {
            // yを反転
            // (x, y) -> (x, MAX_SIZE - y)
            sy = MAX_SIZE - sy;
            gy = MAX_SIZE - gy;
            assert(sy <= gy);
            assert(1 <= sy and sy < MAX_SIZE);
            assert(1 <= gy and gy < MAX_SIZE);
            repeat(i, N) {
                Y[i] = MAX_SIZE - Y[i];
            }
        }

        // sx <= gx, sy <= gyになった

        // スタート地点にかぶっているセンサを削除、答えに追加しておく
        int res = 0;
        repeat(i, N) {
            assert(1 <= X[i] and X[i] < MAX_SIZE);
            assert(1 <= Y[i] and Y[i] < MAX_SIZE);
            if (X[i] - R[i] <= sx and sx <= X[i] + R[i] and Y[i] - R[i] <= sy and sy <= Y[i] + R[i]) {
                X[i] = Y[i] = R[i] = -1;
                ++res;
            }
        }
        //cout << "res: " << res << endl;

        // left[y][x] := (x-1,y) -> (x, y)に入るとき、侵入するセンサ領域の個数
        // up[y][x] := (x, y-1) -> (x, y)に入るとき、侵入するセンサ領域の個数
        repeat(i, N) {
            if (X[i] == -1) continue;
            int cx = X[i] - R[i];
            if (cx < 1) continue;
            int ly = max(1, Y[i] - R[i]);
            int uy = Y[i] + R[i] + 1;
            Left[ly][cx]++;
            if (uy < MAX_SIZE) Left[uy][cx]--;
        }

        repeat(i, N) {
            if (Y[i] == -1) continue;
            int cy = Y[i] - R[i];
            if (cy < 1) continue;
            int lx = max(1, X[i] - R[i]);
            int ux = X[i] + R[i] + 1;
            Up[cy][lx]++;
            if (ux < MAX_SIZE) Up[cy][ux]--;
        }

        repeat(x, MAX_SIZE) {
            for (int y = 1; y < MAX_SIZE; ++y) {
                Left[y][x] += Left[y - 1][x];
            }
        }

        repeat(y, MAX_SIZE) {
            for (int x = 1; x < MAX_SIZE; ++x) {
                Up[y][x] += Up[y][x - 1];
            }
        }

        dp[sy][sx] = 0;
        for (int y = sy; y <= gy; ++y) {
            for (int x = sx; x <= gx; ++x) {
                if (x - 1 >= sx) {
                    dp[y][x] = min(dp[y][x], dp[y][x - 1] + Left[y][x]);
                }
                if (y - 1 >= sy) {
                    dp[y][x] = min(dp[y][x], dp[y - 1][x] + Up[y][x]);
                }
            }
        }
        res += dp[gy][gx];
        cout << res << endl;
    }
    return 0;
}
