#include <bits/stdc++.h>
using namespace std;
using Real = double;
#define rep(i, n) for (int i = 0; i < (n); ++i)
const int DY[4] = {-1, 1, 0, 0}, DX[4] = {0, 0, -1, 1};

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());


/* ---------- 改良版近傍 ---------- */
void neighbor(vector<int> &v, int FREE, double temp) {
    // 温度が高いほど大きく壊す ―― 最大サイズを線形に縮める
    int maxSpan = max(2, int(FREE * (0.20 * temp + 0.03))); // 3%〜23%

    // 0: 挿入, 1: 連続反転, 2: k-cycle shuffle
    int type = rng() % 3;

    if (type == 0) { // -------- ① 挿入 (cut & paste)
        int l = rng() % FREE;
        int r = l + 1 + rng() % maxSpan; // [l, r) を切り取る
        if (r > FREE) r = FREE;
        int dst = rng() % FREE; // 挿入先

        if (dst >= l && dst < r) return; // 動かないのでスキップ

        vector<int> block(v.begin() + l, v.begin() + r);
        v.erase(v.begin() + l, v.begin() + r);
        if (dst > l) dst -= (r - l); // erase 後の補正
        v.insert(v.begin() + dst, block.begin(), block.end());

    } else if (type == 1) { // -------- ② 連続反転 (2-opt 的)
        int l = rng() % FREE;
        int r = l + rng() % maxSpan; // 長さ 1〜maxSpan
        if (r >= FREE) r = FREE - 1;
        if (l < r) std::reverse(v.begin() + l, v.begin() + r + 1);

    } else {                                  // -------- ③ k-cycle shuffle (拡張 swap)
        int k = 3 + rng() % min(10, maxSpan); // 3〜min(10,maxSpan) 点
        vector<int> pos;
        pos.reserve(k);
        while ((int)pos.size() < k) {
            int p = rng() % FREE;
            if (std::find(pos.begin(), pos.end(), p) == pos.end()) pos.push_back(p);
        }
        vector<int> tmp(k);
        for (int i = 0; i < k; ++i) tmp[i] = v[pos[i]];

        // 任意シフト rotate → 巡回置換
        int shift = 1 + rng() % (k - 1);
        std::rotate(tmp.begin(), tmp.begin() + shift, tmp.end());
        for (int i = 0; i < k; ++i) v[pos[i]] = tmp[i];
    }
}

/* ---------- メイン ---------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    const int N2   = n * n;
    const int FREE = N2 - m;

    auto idx = [&](int y, int x) { return y * n + x; };
    auto rev = [&](int id) { return pair{id / n, id % n}; };

    vector<vector<char>> s(n, vector<char>(n));
    rep(i, n) rep(j, n) cin >> s[i][j];
    vector<vector<char>> s_init = s; // 初期盤保存
    vector<int> free_cells;
    free_cells.reserve(FREE);
    rep(i, n) rep(j, n) if (s[i][j] == '.') free_cells.push_back(idx(i, j));

    /* ---------- nxt のビルド・更新 ---------- */
    vector<int> nxt[4];
    rep(d, 4) nxt[d].assign(N2, 0);
    auto build_nxt = [&](const vector<vector<char>> &bd) {
        rep(x, n) {
            int last = -1;
            rep(y, n) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[0][id] = idx(last + 1, x);
            }
            last = n;
            for (int y = n - 1; y >= 0; --y) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[1][id] = idx(last - 1, x);
            }
        }
        rep(y, n) {
            int last = -1;
            rep(x, n) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = x;
                    continue;
                }
                nxt[2][id] = idx(y, last + 1);
            }
            last = n;
            for (int x = n - 1; x >= 0; --x) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = x;
                    continue;
                }
                nxt[3][id] = idx(y, last - 1);
            }
        }
    };
    auto patch_nxt = [&](int y, int x, const vector<vector<char>> &bd) {
        int last = -1;
        rep(i, n) {
            int id = idx(i, x);
            if (bd[i][x] == '#') {
                last = i;
                continue;
            }
            nxt[0][id] = idx(last + 1, x);
        }
        last = n;
        for (int i = n - 1; i >= 0; --i) {
            int id = idx(i, x);
            if (bd[i][x] == '#') {
                last = i;
                continue;
            }
            nxt[1][id] = idx(min(last - 1, n - 1), x);
        }
        last = -1;
        rep(j, n) {
            int id = idx(y, j);
            if (bd[y][j] == '#') {
                last = j;
                continue;
            }
            nxt[2][id] = idx(y, last + 1);
        }
        last = n;
        for (int j = n - 1; j >= 0; --j) {
            int id = idx(y, j);
            if (bd[y][j] == '#') {
                last = j;
                continue;
            }
            nxt[3][id] = idx(y, min(last - 1, n - 1));
        }
    };

    /* ---------- 1 手進め ---------- */
    vector<Real> buf(N2);
    auto step_prob = [&](vector<Real> &prob, const vector<vector<char>> &bd) {
        fill(buf.begin(), buf.end(), 0.0);
        rep(id, N2) {
            if (bd[id / n][id % n] == '#') continue;
            Real w = prob[id];
            if (w == 0.0) continue;
            rep(d, 4) buf[nxt[d][id]] += w * 0.25;
        }
        prob.swap(buf);
    };

    /* ----------------------------------------------------------------
       1) 先読み 2 手貪欲で初期列 P を作成（提供コードそのまま）
    ---------------------------------------------------------------- */
    vector<Real> q(N2, 1.0), v0(N2), v1(N2), v2(N2);
    rep(id, N2) if (s[id / n][id % n] == '#') q[id] = 0.0;
    build_nxt(s);

    vector<int> P;
    P.reserve(FREE);

    rep(turn, FREE) {
        rep(id, N2) v0[id] = (s[id / n][id % n] == '#') ? 0.0 : q[id];
        fill(v1.begin(), v1.end(), 0.0);
        rep(id, N2) if (v0[id]) {
            Real sh = v0[id] * 0.25;
            rep(d, 4) v1[nxt[d][id]] += sh;
        }
        fill(v2.begin(), v2.end(), 0.0);
        rep(id, N2) if (v1[id]) {
            Real sh = v1[id] * 0.25;
            rep(d, 4) v2[nxt[d][id]] += sh;
        }
        int best   = -1;
        Real bestF = 1e300;
        rep(id, N2) if (s[id / n][id % n] != '#') {
            Real f = v0[id] + v1[id] + v2[id];
            if (f < bestF) {
                bestF = f;
                best  = id;
            }
        }
        P.push_back(best);
        int y = best / n, x = best % n;
        s[y][x] = '#';
        q[best] = 0.0;
        patch_nxt(y, x, s);
        step_prob(q, s);
    }

    /* ----------------------------------------------------------------
       2) 評価関数：期待賞金を正確に計算
    ---------------------------------------------------------------- */
    auto evaluate = [&](const vector<int> &order) -> double {
        vector<vector<char>> bd = s_init;
        build_nxt(bd);
        vector<Real> prob(N2, 0.0);
        for (int id : free_cells) prob[id] = 1.0 / FREE;

        double score = 0.0, alive = 1.0;

        for (int t = 0; t < FREE; ++t) {
            step_prob(prob, bd);
            int pos     = order[t];
            Real p_here = prob[pos]; // 潰れる確率
            score += alive - p_here; // 生き残りなら +1
            alive -= p_here;
            if (alive <= 1e-12) break;
            prob[pos]   = 0.0;
            auto [y, x] = rev(pos);
            bd[y][x]    = '#';
            patch_nxt(y, x, bd);
        }
        return score;
    };

    /* ----------------------------------------------------------------
       3) 焼きなまし （k-swap のみ）
    ---------------------------------------------------------------- */
    vector<int> bestP = P;
    double bestScore  = evaluate(bestP);

    auto now_ms = []() {
        return chrono::duration_cast<chrono::milliseconds>(
                   chrono::steady_clock::now().time_since_epoch())
            .count();
    };
    const long long TIME = 1800;
    long long t0         = now_ms();

    std::uniform_real_distribution<double> uni(0.0, 1.0);

    while (now_ms() - t0 < TIME) {
        double prog = (now_ms() - t0) / (double)TIME;     // 0 → 1
        double temp = 1e-3 + (1.0 - 1e-3) * (1.0 - prog); // 線形温度

        vector<int> cand = bestP;
        neighbor(cand, FREE, temp);

        double candScore = evaluate(cand);
        double delta     = candScore - bestScore;
        if (delta > 0 || exp(delta / temp) > uni(rng)) {
            bestP.swap(cand);
            bestScore = candScore;
        }
    }

    /* ----------------------------------------------------------------
       4) 出力
    ---------------------------------------------------------------- */
    for (int id : bestP) { cout << id / n << " " << id % n << '\n'; }
    return 0;
}
