#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
const string T = "atcoder";

// a に b を足して、MOD をとる関数
void add(int &a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
}

int main() {
    int N;
    string S;
    cin >> N >> S;

    // DP テーブル
    vector<vector<int>> dp(N+1, vector<int>(T.size()+1, 0));

    // 初期条件
    dp[0][0] = 1;

    // ループ
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= T.size(); ++j) {
            // S[i] を選ばない場合
            add(dp[i+1][j], dp[i][j]);

            // S[i] を選ぶ場合
            if (j < T.size() && S[i] == T[j]) {
                add(dp[i+1][j+1], dp[i][j]);
            }
        }
    }
    cout << dp[N][T.size()] << endl;
}