#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define el '\n'

/*
  Simulated Annealing solver for AtCoder AHC “Skating Rink”
  - greedy initial sequence with 3‑step look‑ahead
  - neighbourhood: swap / reverse / insert
  - temperature‑schedule: geometric (α = 0.995)
  Compile: g++ -std=c++20 -O2 -march=native main.cpp
*/

const int DY[4] = {-1, 1, 0, 0}, DX[4] = {0, 0, -1, 1};
std::mt19937 rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());

int n, m;
std::vector<std::vector<char>> initial_s;

inline int idx(int y, int x) { return y * n + x; }

/* ---------------- reachable‑next table ---------------- */
std::vector<std::vector<int>> build_nxt(const std::vector<std::vector<char>> &s) {
    std::vector<std::vector<int>> nxt(4, std::vector<int>(n * n));
    /* ↑ ↓ */
    rep(x, n) {
        int last = -1;
        rep(y, n) {
            int id = idx(y, x);
            if (s[y][x] == '#') {
                last = y;
                continue;
            }
            nxt[0][id] = idx(last + 1, x);
        }
        last = n;
        for (int y = n - 1; y >= 0; --y) {
            int id = idx(y, x);
            if (s[y][x] == '#') {
                last = y;
                continue;
            }
            nxt[1][id] = idx(last - 1, x);
        }
    }
    /* ← → */
    rep(y, n) {
        int last = -1;
        rep(x, n) {
            int id = idx(y, x);
            if (s[y][x] == '#') {
                last = x;
                continue;
            }
            nxt[2][id] = idx(y, last + 1);
        }
        last = n;
        for (int x = n - 1; x >= 0; --x) {
            int id = idx(y, x);
            if (s[y][x] == '#') {
                last = x;
                continue;
            }
            nxt[3][id] = idx(y, last - 1);
        }
    }
    return nxt;
}

/* ---------------- fast approximate evaluation ---------------- */
double compute_score(const std::vector<std::pair<int, int>> &P) {
    std::vector<std::vector<char>> s = initial_s;
    std::vector<long long> q(n * n, 1), buf(n * n);
    rep(i, n) rep(j, n) if (s[i][j] == '#') q[idx(i, j)] = 0;
    auto nxt                                             = build_nxt(s);

    double score = 0.0;
    for (auto [y, x] : P) {
        int id        = idx(y, x);
        long long tot = std::accumulate(q.begin(), q.end(), 0LL);
        if (!tot) break;
        score += 1.0 - (double)q[id] / tot;

        s[y][x] = '#';
        q[id]   = 0;
        nxt     = build_nxt(s);

        std::fill(buf.begin(), buf.end(), 0LL);
        rep(i, n * n) {
            if (s[i / n][i % n] == '#') continue;
            long long w = q[i];
            if (!w) continue;
            long long share = (w + 3) >> 2; // ⌈w/4⌉
            rep(d, 4) buf[nxt[d][i]] += share;
        }
        q.swap(buf);
    }
    return score;
}

/* ---------------- simulated annealing ---------------- */
std::vector<std::pair<int, int>> anneal(std::vector<std::pair<int, int>> P) {
    double cur  = compute_score(P);
    double best = cur;
    auto bestP  = P;
    double T = 100.0, Tmin = 1e-3, alpha = 0.995;
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    for (int iter = 0; T > Tmin && iter < 100000; ++iter) {
        if (iter % 100 == 0) T *= alpha;

        std::vector<std::pair<int, int>> Q = P;
        int a = rng() % Q.size(), b = rng() % Q.size();
        if (a > b) std::swap(a, b);
        int op = rng() % 3;
        if (op == 0) std::swap(Q[a], Q[b]);                               // swap
        else if (op == 1) std::reverse(Q.begin() + a, Q.begin() + b + 1); // reverse
        else {
            auto tmp = Q[a];
            Q.erase(Q.begin() + a);
            Q.insert(Q.begin() + b, tmp); // insert
        }

        double scr   = compute_score(Q);
        double delta = scr - cur;
        if (delta > 0 || std::exp(delta / T) > uni(rng)) {
            P   = std::move(Q);
            cur = scr;
            if (cur > best) {
                best  = cur;
                bestP = P;
            }
        }
    }
    return bestP;
}

/* ---------------- main ---------------- */
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    if (!(std::cin >> n >> m)) return 0;
    initial_s.assign(n, std::vector<char>(n));
    rep(i, n) rep(j, n) std::cin >> initial_s[i][j];

    /* greedy initial sequence with 3‑step look‑ahead */
    std::vector<std::vector<char>> s = initial_s;
    std::vector<long long> q(n * n, 1), buf(n * n);
    rep(i, n) rep(j, n) if (s[i][j] == '#') q[idx(i, j)] = 0;
    auto nxt                                             = build_nxt(s);

    int free_cnt = n * n - m;
    std::vector<double> v[4];
    rep(i, 4) v[i].assign(n * n, 0.0);
    std::vector<std::pair<int, int>> P;
    P.reserve(free_cnt);

    rep(turn, free_cnt) {
        rep(id, n * n) v[0][id] = (s[id / n][id % n] == '#') ? 0.0 : (double)q[id];
        rep(step, 3) {
            std::fill(v[step + 1].begin(), v[step + 1].end(), 0.0);
            rep(id, n * n) {
                double p = v[step][id];
                if (!p) continue;
                double share = p * 0.25;
                rep(d, 4) v[step + 1][nxt[d][id]] += share;
            }
        }
        int best     = -1;
        double bestF = 1e300;
        rep(id, n * n) if (s[id / n][id % n] != '#') {
            double f = v[0][id] + v[1][id] + v[2][id] + v[3][id];
            if (f < bestF) bestF = f, best = id;
        }
        int y = best / n, x = best % n;
        P.emplace_back(y, x);

        /* apply move */
        s[y][x] = '#';
        q[best] = 0;
        nxt     = build_nxt(s);
        std::fill(buf.begin(), buf.end(), 0LL);
        rep(id, n * n) if (s[id / n][id % n] != '#') {
            long long w = q[id];
            if (!w) continue;
            long long share = (w + 3) >> 2;
            rep(d, 4) buf[nxt[d][id]] += share;
        }
        q.swap(buf);
    }

    /* hill‑climb with simulated annealing */
    P = anneal(std::move(P));

    for (auto [y, x] : P) std::cout << y << ' ' << x << el;
    return 0;
}
