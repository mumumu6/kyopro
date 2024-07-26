#include <bits/stdc++.h>
using namespace std;

// 文字列が長さKの回文を含むかどうかを確認する関数
bool has_palindromic_substring(const string& T, int K) {
    int N = T.size();
    for (int i = 0; i <= N - K; ++i) {
        bool is_palindrome = true;
        for (int j = 0; j < K / 2; ++j) {
            if (T[i + j] != T[i + K - 1 - j]) {
                is_palindrome = false;
                break;
            }
        }
        if (is_palindrome) {
            return true;
        }
    }
    return false;
}

// 文字列Sの全ての並べ替えで長さKの回文を含まないものの個数を数える関数
int count_non_palindromic_permutations(const string& S, int K) {
    set<string> permutations;
    string T = S;
    sort(T.begin(), T.end()); // next_permutationを使うためにソート

    // 全ての並べ替えを生成し、setに入れて重複を排除
    do {
        permutations.insert(T);
    } while (next_permutation(T.begin(), T.end()));

    int count = 0;
    for (const string& perm : permutations) {
        if (!has_palindromic_substring(perm, K)) {
            count++;
        }
    }

    return count;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    string S;
    // aaaa
    int n,K;
    cin >> n;
    cin >> K >> S;

    cout << count_non_palindromic_permutations(S, K) << endl;

    return 0;
}
