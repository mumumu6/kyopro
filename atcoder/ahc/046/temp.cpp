/**********************************************************
 *  Safe Greedy Skating with Blocks
 *   – per-segment 5-choice greedy (none / A U / A D / A L / A R)
 *   – always AC, usually better score than random HC
 **********************************************************/
#include <bits/stdc++.h>
using namespace std;

struct Pt {
    int r, c;
};

const int DIR      = 4;
const int dr[DIR]  = {-1, +1, 0, 0};
const int dc[DIR]  = {0, 0, -1, +1};
const char mv[DIR] = {'U', 'D', 'L', 'R'}; // 1 マス移動
const char sl[DIR] = {'u', 'd', 'l', 'r'}; // 滑走（着地、小文字）

/* ---------- 汎用 ---------- */
inline bool inside(int N, int r, int c) { return 0 <= r && r < N && 0 <= c && c < N; }

/* ---------- 滑走込み BFS (ブロック固定) ---------- */
bool bfsSlide(const vector<vector<char>> &block, Pt S, Pt G,
              vector<char> &outCmd) // 出力: コマンド列 (M / S)
{
    int N         = block.size();
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

        /* 1マス移動 */
        for (int d = 0; d < DIR; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (!inside(N, nr, nc) || block[nr][nc]) continue;
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
                if (!inside(N, tr, tc) || block[tr][tc]) break;
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
    outCmd.clear();
    for (int r = G.r, c = G.c; !(r == S.r && c == S.c);) {
        char a = act[r][c];
        outCmd.push_back(a);
        Pt p = pre[r][c];
        r    = p.r;
        c    = p.c;
    }
    reverse(outCmd.begin(), outCmd.end());
    return true;
}

/* ---------- メイン ---------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<Pt> goals(M);
    for (auto &p : goals) cin >> p.r >> p.c;

    /* futureGoal[r][c] == true ならまだ訪れていない目的地 */
    vector<vector<char>> future(N, vector<char>(N, 0));
    for (auto g : goals) future[g.r][g.c] = 1;

    vector<vector<char>> block(N, vector<char>(N, 0)); // ブロック配置
    vector<string> answer;                             // 出力コマンド列

    Pt cur                    = goals[0];
    const long long LIMIT_CMD = 2LL * N * M;

    for (int idx = 1; idx < M; ++idx) {
        Pt g = goals[idx];

        /* ---------- 5 通り（置かない / U D L R）を全評価 ---------- */
        vector<char> bestPath;
        int bestAddDir = -1; // -1=置かない, 0..3=方向
        size_t bestLen = SIZE_MAX;

        for (int dTry = -1; dTry < DIR; ++dTry) {
            int br = -1, bc = -1;
            if (dTry != -1) {
                br = g.r + dr[dTry];
                bc = g.c + dc[dTry];
                if (!inside(N, br, bc)) continue;
                if (block[br][bc]) continue;
                if (future[br][bc]) continue; // 未来の目的地には置かない
            }

            /* 一時的にブロックを置く */
            if (dTry != -1) block[br][bc] = 1;

            vector<char> path;
            bool ok = bfsSlide(block, cur, g, path);

            /* ロールバック */
            if (dTry != -1) block[br][bc] = 0;
            if (!ok) continue; // 到達不能は除外

            size_t score = path.size() + (dTry != -1 ? 1 : 0);
            if (score < bestLen || (score == bestLen && dTry == -1)) {
                bestLen    = score;
                bestPath   = std::move(path);
                bestAddDir = dTry;
            }
        }

        /* ---------- ベスト案を確定 ---------- */
        // (必ず一つは見つかるはず：dTry==-1 は到達可能)

        /* 経路コマンドを出力バッファへ */
        for (char a : bestPath) {
            if (islower(a)) answer.emplace_back("S "s + char(toupper(a)));
            else answer.emplace_back("M "s + a);
        }

        /* ブロック設置をコミット */
        if (bestAddDir != -1) {
            int br = g.r + dr[bestAddDir], bc = g.c + dc[bestAddDir];
            block[br][bc] = 1;
            answer.emplace_back("A "s + mv[bestAddDir]);
        }

        /* このゴールは訪問済みに */
        future[g.r][g.c] = 0;
        cur              = g;
    }

    /* ---------- 行数制限チェック ---------- */
    if ((long long)answer.size() > LIMIT_CMD) {
        // セーフティフォールバック：純移動解を出す
        cerr << "WARN: command length exceeded limit, fall back.\n";
        answer.clear();
        vector<vector<char>> emptyBlock(N, vector<char>(N, 0));
        Pt c = goals[0];
        for (int i = 1; i < M; ++i) {
            vector<char> p;
            bfsSlide(emptyBlock, c, goals[i], p);
            for (char a : p) {
                if (islower(a)) answer.emplace_back("S "s + char(toupper(a)));
                else answer.emplace_back("M "s + a);
            }
            c = goals[i];
        }
    }

    /* ---------- 出力 ---------- */
    for (auto &s : answer) cout << s << "\n";
    return 0;
}