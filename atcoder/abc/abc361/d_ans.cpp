#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    string S, T;
    cin >> N >> S >> T;

    S += "..";
    T += "..";
    map<string, int> dp;
    dp[S] = 0;
    queue<string> que;
    que.push(S);
    while (!que.empty()) {
        auto str = que.front();
        que.pop();

        int emp = 0;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '.') {
                emp = i;
                break;
            }
        }

        for (int i = 0; i + 1 < str.size(); ++i) {
            if (str[i] == '.' || str[i + 1] == '.') continue;
            string str2 = str;
            swap(str2[i], str2[emp]);
            swap(str2[i + 1], str2[emp + 1]);
            if (!dp.count(str2)) {
                dp[str2] = dp[str] + 1;
                que.push(str2);
                // cout << str2 << endl;
            }
        }
    }
    if (dp.count(T))
        cout << dp[T] << endl;
    else
        cout << -1 << endl;
}