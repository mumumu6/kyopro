#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int N = 36, M = 12; // 固定
    int L;                    // 10^6 だが使わない
    vector<string> S(N);
    vector<int> P(N);
    cin >> ws; // 先頭の N は読み捨て
    int dummyN;
    cin >> dummyN; // =36
    int dummyM;
    cin >> dummyM; // =12
    cin >> L;
    for (int i = 0; i < N; i++) cin >> S[i] >> P[i];

    // ① トップ文字列
    vector<int> ord(N);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b) { return P[a] > P[b]; });
    const string top = S[ord[0]];
    const int D      = top.size();

    // 文字割り当て
    vector<char> C(M, 'a');
    for (int i = 0; i < D; i++) C[i] = top[i];
    string alph = "abcdef";
    int pos     = D;
    for (char ch : alph)
        if (pos < M && top.find(ch) == string::npos) C[pos++] = ch;

    // 遷移行列
    vector<vector<int>> A(M, vector<int>(M, 0));
    // 主サイクル 70%
    for (int i = 0; i < D; i++) A[i][(i + 1) % D] += 40;

    // ② その他文字列用の入口
    long long sumP = 0;
    for (int k = 1; k < N; k++) sumP += P[ord[k]];
    for (int k = 1; k < N; k++) {
        int idx    = ord[k];
        char first = S[idx][0], second = S[idx][1];
        int s = -1, t = -1;
        for (int i = 0; i < M; i++)
            if (C[i] == first) s = i;
        for (int i = 0; i < M; i++)
            if (C[i] == second) t = i;
        if (s < 0 || t < 0) continue; // 失敗したら捨て
        int w = int((60.0 * P[idx]) / sumP + 0.5);
        A[s][t] += w;
    }

    // 残りを自ループ
    for (int i = 0; i < M; i++) {
        int sm = 0;
        for (int j = 0; j < M; j++) sm += A[i][j];
        if (sm < 100) A[i][i] += 100 - sm;
    }

    // ---- 出力 ----
    for (int i = 0; i < M; i++) {
        cout << C[i];
        for (int j = 0; j < M; j++) cout << ' ' << A[i][j];
        cout << "\n";
    }
    return 0;
}
