#include <bits/stdc++.h>
using namespace std;
using Real = double;
#define rep(i, n) for (int i = 0; i < (n); ++i)

/* ---------- 乱数 ---------- */
std::mt19937 rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());

/* ---------- nxt テーブル ---------- */
struct Nxt {
    int n, N2;
    vector<int> nxt[4];
    Nxt(int n) : n(n), N2(n * n) { rep(d, 4) nxt[d].assign(N2, 0); }
    inline int idx(int y, int x) const { return y * n + x; }

    void build(const vector<vector<char>> &bd) {
        rep(x, n) {
            int last = -1;
            rep(y, n) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[0][id] = idx(last + 1, x); // ↑
            }
            last = n;
            for (int y = n - 1; y >= 0; --y) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = y;
                    continue;
                }
                nxt[1][id] = idx(last - 1, x); // ↓
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
                nxt[2][id] = idx(y, last + 1); // ←
            }
            last = n;
            for (int x = n - 1; x >= 0; --x) {
                int id = idx(y, x);
                if (bd[y][x] == '#') {
                    last = x;
                    continue;
                }
                nxt[3][id] = idx(y, last - 1); // →
            }
        }
    }
    void patch(int y, int x, const vector<vector<char>> &bd) {
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
            nxt[1][id] = idx(last - 1, x);
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
            nxt[3][id] = idx(y, last - 1);
        }
    }
};

/* ---------- 1 手進める ---------- */
void step_prob(const Nxt &T, const vector<vector<char>> &bd, vector<double> &p, vector<double> &buf) {
    fill(buf.begin(), buf.end(), 0.0);
    int N2 = T.N2;
    rep(id, N2) {
        if (bd[id / T.n][id % T.n] == '#') continue;
        double w = p[id];
        if (w == 0.0) continue;
        rep(d, 4) buf[T.nxt[d][id]] += w * 0.25;
    }
    p.swap(buf);
}

/* =========================================================
   evaluate_exact : 期待賞金 E
   ========================================================= */
double evaluate_exact(int n, const vector<vector<char>> &s_init, const vector<int> &order) {
    int N2 = n * n, FREE = (int)order.size();
    vector<vector<char>> bd = s_init;
    Nxt T(n);
    T.build(bd);

    vector<double> prob(N2, 0.0), buf(N2);
    for (int id : order)
        if (bd[id / n][id % n] == '.') prob[id] = 1.0 / FREE;

    double score = 0.0, alive = 1.0;
    rep(t, FREE) {
        step_prob(T, bd, prob, buf);
        int pos      = order[t];
        double p_die = prob[pos];
        score += alive - p_die;
        alive -= p_die;
        if (alive <= 1e-12) break;

        prob[pos]            = 0.0;
        bd[pos / n][pos % n] = '#';
        T.patch(pos / n, pos % n, bd);
    }
    return score;
}

/* =========================================================
   make_exact_greedy : evaluate と全く同じロジックで列を構築
   TOP_K 上位から乱択
   ========================================================= */
vector<int> make_exact_greedy(int n, const vector<vector<char>> &s_init, const vector<int> &free_cells,
                              int TOP_K = 5) {
    int N2 = n * n, FREE = (int)free_cells.size();
    vector<vector<char>> bd = s_init;
    Nxt T(n);
    T.build(bd);

    vector<double> prob(N2, 0.0), buf(N2);
    for (int id : free_cells) prob[id] = 1.0 / FREE;

    /* --- 作成する順序 --- */
    vector<int> order;
    order.reserve(FREE);
    double alive = 1.0;

    for (int turn = 0; turn < FREE && alive > 1e-12; ++turn) {
        /* 1) まず確率を 1 手進める */
        step_prob(T, bd, prob, buf);

        /* 2) p_die の小さい TOP_K を収集 */
        vector<pair<double, int>> cand;
        cand.reserve(FREE - turn);
        rep(id, N2) if (bd[id / n][id % n] != '#') cand.emplace_back(prob[id], id);

        if (cand.empty()) break; // 念のため
        int limit = min(TOP_K, (int)cand.size());
        if (FREE - turn < 400) limit = min(10, (int)cand.size());

        nth_element(cand.begin(), cand.begin() + limit - 1, cand.end(),
                    [](auto &a, auto &b) { return a.first < b.first; });

        int best = cand[rng() % limit].second; // 3) 上位から乱択

        /* 4) 盤面＆確率を evaluate と同じ手順で更新 */
        order.push_back(best);
        double p_die = prob[best];
        alive -= p_die;
        prob[best] = 0.0;
        int y = best / n, x = best % n;
        bd[y][x] = '#';
        T.patch(y, x, bd);
    }

    return order;
}

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
/* =========================================================
   main
   ========================================================= */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int N2 = n * n;
    vector<vector<char>> s(n, vector<char>(n));
    rep(i, n) rep(j, n) cin >> s[i][j];

    vector<int> free_cells;
    free_cells.reserve(N2 - m);
    rep(i, n) rep(j, n) if (s[i][j] == '.') free_cells.push_back(i * n + j);

    const long long LIMIT_MS = 8000;
    auto now_ms              = []() {
        return chrono::duration_cast<chrono::milliseconds>(
                   chrono::steady_clock::now().time_since_epoch())
            .count();
    };
    long long start = now_ms();

    vector<int> best_ord;
    double best_sc = -1.0;

    while (now_ms() - start < LIMIT_MS) {
        auto ord  = make_exact_greedy(n, s, free_cells, 30);
        double sc = evaluate_exact(n, s, ord);
        if (sc > best_sc) {
            best_sc = sc;
            best_ord.swap(ord);
        }
    }

    start = now_ms();

    int FREE = n * n - m;
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    while (now_ms() - start < LIMIT_MS) {
        double prog = (now_ms() - start) / (double)LIMIT_MS; // 0 → 1
        double temp = 1e-3 + (1.0 - 1e-3) * (1.0 - prog);    // 線形温度

        vector<int> cand = best_ord;
        neighbor(cand, FREE, temp);

        double candScore = evaluate_exact(n,s,cand);
        double delta     = candScore - best_sc;
        if (delta > 0 || exp(delta / temp) > uni(rng)) {
            best_ord.swap(cand);
            best_sc = candScore;
        }
    }

    for (int id : best_ord) cout << id / n << " " << id % n << "\n";
}
