#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;

static const int DX[4] = {-1, 1, 0, 0};
static const int DY[4] = {0, 0, -1, 1};

struct XorShift {
    uint64_t x = 88172645463325252ull;
    uint64_t next_u64() {
        x ^= x << 7;
        x ^= x >> 9;
        return x;
    }
    int next_int(int n) {
        return (int)(next_u64() % (uint64_t)n);
    }
};

static inline bool inside(int x, int y, int n) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

vector<pii> get_candidates(
    int player,
    int n,
    int m,
    const vector<vector<int>> &owner,
    const vector<int> &px,
    const vector<int> &py
) {
    vector<vector<char>> reachable(n, vector<char>(n, 0));
    queue<pii> q;
    reachable[px[player]][py[player]] = 1;
    q.emplace(px[player], py[player]);
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nx = x + DX[d];
            int ny = y + DY[d];
            if (!inside(nx, ny, n)) continue;
            if (reachable[nx][ny]) continue;
            if (owner[nx][ny] != player) continue;
            reachable[nx][ny] = 1;
            q.emplace(nx, ny);
        }
    }

    vector<vector<char>> can(n, vector<char>(n, 0));
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            if (!reachable[x][y]) continue;
            can[x][y] = 1;
            for (int d = 0; d < 4; d++) {
                int nx = x + DX[d];
                int ny = y + DY[d];
                if (inside(nx, ny, n)) can[nx][ny] = 1;
            }
        }
    }

    for (int p = 0; p < m; p++) {
        if (p == player) continue;
        can[px[p]][py[p]] = 0;
    }

    vector<pii> cand;
    cand.reserve(n * n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            if (can[x][y]) cand.emplace_back(x, y);
        }
    }
    return cand;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    int N, M, T, U;
    cin >> N >> M >> T >> U;
    vector<vector<int>> V(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> V[i][j];
        }
    }

    vector<int> px(M), py(M), sx(M), sy(M);
    for (int p = 0; p < M; p++) {
        cin >> sx[p] >> sy[p];
        px[p] = sx[p];
        py[p] = sy[p];
    }

    vector<vector<int>> owner(N, vector<int>(N, -1));
    vector<vector<int>> level(N, vector<int>(N, 0));
    for (int p = 0; p < M; p++) {
        owner[sx[p]][sy[p]] = p;
        level[sx[p]][sy[p]] = 1;
    }

    ll sumV = 0, sumX = 0, sumY = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sumV += V[i][j];
            sumX += 1LL * i * V[i][j];
            sumY += 1LL * j * V[i][j];
        }
    }
    double cx = (double)sumX / (double)sumV;
    double cy = (double)sumY / (double)sumV;

    XorShift rng;
    for (int turn = 0; turn < T; turn++) {
        vector<vector<vector<char>>> enemy_can(
            max(0, M - 1),
            vector<vector<char>>(N, vector<char>(N, 0))
        );
        for (int p = 1; p < M; p++) {
            auto c = get_candidates(p, N, M, owner, px, py);
            for (auto [x, y] : c) enemy_can[p - 1][x][y] = 1;
        }

        auto my_cand = get_candidates(0, N, M, owner, px, py);
        if (my_cand.empty()) {
            cout << px[0] << ' ' << py[0] << '\n' << flush;
        } else {
            double best_score = -1e100;
            vector<pii> best_moves;
            for (auto [x, y] : my_cand) {
                double s = 0.0;
                int own = owner[x][y];
                int lv = level[x][y];
                int vv = V[x][y];

                if (own == -1) s += 2.20 * vv;
                else if (own == 0) {
                    if (lv < U) s += 1.15 * vv;
                    else s += 0.05 * vv;
                } else {
                    if (lv == 1) s += 2.00 * vv;
                    else s += 0.35 * vv;
                }

                for (int d = 0; d < 4; d++) {
                    int nx = x + DX[d];
                    int ny = y + DY[d];
                    if (!inside(nx, ny, N)) continue;
                    if (owner[nx][ny] == -1) s += 0.35 * V[nx][ny];
                    else if (owner[nx][ny] == 0 && level[nx][ny] < U) s += 0.15 * V[nx][ny];
                    else if (owner[nx][ny] != 0) {
                        if (level[nx][ny] == 1) s += 0.45 * V[nx][ny];
                        else s += 0.08 * V[nx][ny];
                    }
                }

                if (own != 0) {
                    int danger = 0;
                    for (int p = 1; p < M; p++) {
                        if (enemy_can[p - 1][x][y]) {
                            danger++;
                            if (own == p) danger++;
                        }
                    }
                    s -= 0.45 * danger * vv;
                }

                if (x == px[0] && y == py[0]) s -= 40.0;

                double dist_center = abs(x - cx) + abs(y - cy);
                s -= 3.0 * dist_center;

                if (s > best_score + 1e-9) {
                    best_score = s;
                    best_moves.clear();
                    best_moves.emplace_back(x, y);
                } else if (fabs(s - best_score) <= 1e-9) {
                    best_moves.emplace_back(x, y);
                }
            }

            pii mv = best_moves[rng.next_int((int)best_moves.size())];
            cout << mv.first << ' ' << mv.second << '\n' << flush;
        }

        vector<int> tx(M), ty(M);
        for (int p = 0; p < M; p++) cin >> tx[p] >> ty[p];
        for (int p = 0; p < M; p++) cin >> px[p] >> py[p];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) cin >> owner[i][j];
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) cin >> level[i][j];
        }
    }
    return 0;
}
