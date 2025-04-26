/************************************************************
 *  Skating with Blocks – safe AC + simple hill-climb
 *  (C++20, one-file, AC guaranteed)
 ***********************************************************/
#include <bits/stdc++.h>
using namespace std;
struct Pt {
    int r, c;
};

const int DIR      = 4;
const int dr[DIR]  = {-1, +1, 0, 0};
const int dc[DIR]  = {0, 0, -1, +1};
const char mv[DIR] = {'U', 'D', 'L', 'R'};
const char sl[DIR] = {'u', 'd', 'l', 'r'}; // 滑走着地用

/* ---------- BFS: 移動 1 / 滑走 1 ---------- */
bool bfs_slide(int N, const vector<vector<char>> &block, Pt S, Pt G,
               vector<char> &path) // ←結果を返す
{
    const int INF = 1e9;
    struct Node {
        int r, c;
    };
    vector<vector<int>> dist(N, vector<int>(N, INF));
    vector<vector<Pt>> pre(N, vector<Pt>(N, {-1, -1}));
    vector<vector<char>> act(N, vector<char>(N, '?'));

    queue<Node> q;
    dist[S.r][S.c] = 0;
    q.push({S.r, S.c});

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == G.r && c == G.c) break;

        /* 1 マス移動 */
        for (int d = 0; d < DIR; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (block[nr][nc]) continue;
            if (dist[nr][nc] <= dist[r][c] + 1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            pre[nr][nc]  = {r, c};
            act[nr][nc]  = mv[d];
            q.push({nr, nc});
        }
        /* 滑走 */
        for (int d = 0; d < DIR; ++d) {
            int nr = r, nc = c;
            while (true) {
                int tr = nr + dr[d], tc = nc + dc[d];
                if (tr < 0 || tr >= N || tc < 0 || tc >= N || block[tr][tc]) break;
                nr = tr;
                nc = tc;
            }
            if (nr == r && nc == c) continue;
            if (dist[nr][nc] <= dist[r][c] + 1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            pre[nr][nc]  = {r, c};
            act[nr][nc]  = sl[d];
            q.push({nr, nc});
        }
    }
    if (dist[G.r][G.c] == INF) return false; // 到達不能

    /* 経路復元 */
    path.clear();
    for (int r = G.r, c = G.c; !(r == S.r && c == S.c);) {
        char a = act[r][c];
        path.push_back(a);
        Pt p = pre[r][c];
        r    = p.r;
        c    = p.c;
    }
    reverse(path.begin(), path.end());
    return true;
}

/* ---------- 解を構築しスコアを返す (失敗時 score=INT_MAX) ---------- */
struct Result {
    vector<string> cmdLines;
    int score; // 行数

    void swap(Result &other) {
        cmdLines.swap(other.cmdLines);
        std::swap(score, other.score);
    }
};
Result build_solution(int N, const vector<Pt> &goals,
                      const vector<int> &plan) // size M-1, {-1,0..3}
{
    vector<vector<char>> block(N, vector<char>(N, 0));
    vector<vector<char>> future(N, vector<char>(N, 0));
    for (auto p : goals) future[p.r][p.c] = 1;

    vector<string> out;
    Pt cur = goals[0];

    for (size_t idx = 1; idx < goals.size(); ++idx) {
        Pt g             = goals[idx];
        future[g.r][g.c] = 0; // 今回訪れる地点を future から外す

        /* 経路探索 */
        vector<char> path;
        if (!bfs_slide(N, block, cur, g, path)) return {{}, INT_MAX};
        for (char a : path) {
            if (islower(a)) out.emplace_back("S "s + char(toupper(a)));
            else out.emplace_back("M "s + a);
        }

        /* ブロック設置 (plan[idx-1]) */
        int dir = plan[idx - 1];
        if (dir != -1) {
            int br = g.r + dr[dir], bc = g.c + dc[dir];
            if (0 <= br && br < N && 0 <= bc && bc < N && !block[br][bc] && !future[br][bc]) {
                block[br][bc] = 1;
                out.emplace_back("A "s + mv[dir]);
            }
        }
        cur = g;
    }
    int LIM = 2 * N * (int)goals.size();
    if ((int)out.size() > LIM) return {{}, INT_MAX}; // 操作数オーバー

    return {out, (int)out.size()};
}

/* ---------- 単純 (ブロック無し) 解 ---------- */
Result build_naive(int N, const vector<Pt> &goals) {
    vector<string> out;
    Pt cur = goals[0];
    for (size_t idx = 1; idx < goals.size(); ++idx) {
        Pt g    = goals[idx];
        int drw = g.r - cur.r;
        int dcw = g.c - cur.c;
        char vr = (drw < 0 ? 'U' : 'D');
        char hr = (dcw < 0 ? 'L' : 'R');
        for (int k = 0; k < abs(drw); ++k) out.emplace_back("M "s + vr);
        for (int k = 0; k < abs(dcw); ++k) out.emplace_back("M "s + hr);
        cur = g;
    }
    return {out, (int)out.size()};
}

/* ---------- メイン ---------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* 入力 */
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<Pt> goals(M);
    for (auto &p : goals) cin >> p.r >> p.c;

    /* 初期プラン: すべて -1 (=置かない) */
    vector<int> plan(M - 1, -1);
    auto best = build_solution(N, goals, plan);

    /* 山登りでブロック向きを改良 */
    std::mt19937_64 rng(20250426);
    std::uniform_int_distribution<int> dSeg(0, M - 2);
    std::uniform_int_distribution<int> dDir(-1, 3);

    auto start      = chrono::steady_clock::now();
    const double TL = 1950.0; // ms

    while (true) {
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - start).count();
        if (ms > TL) break;

        int sg  = dSeg(rng);
        int old = plan[sg];
        int neu;
        do { neu = dDir(rng); } while (neu == old);
        plan[sg] = neu;

        auto cand = build_solution(N, goals, plan);
        if (cand.score < best.score) {
            swap(best, cand); // 改善
        } else {
            plan[sg] = old; // 戻す
        }
    }

    /* フォールバック: Naive の方が安全なら置き換える */
    auto naive = build_naive(N, goals);
    if (naive.score < best.score || best.score == INT_MAX) { swap(best, naive); }

    for (auto &s : best.cmdLines) cout << s << "\n";
    return 0;
}
