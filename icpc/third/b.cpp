#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
int main() {
    int L;
    cin >> L;
    vector<char> s(L);
    rep(i, L) cin >> s[i];
    vector<int> check(L, 2);

    rep(i, L) { // ３あったら確定なので
        if (s[i] == '3') {
            check[i] = 1;
            if (i > 0) check[i - 1] = 1;
            if (i < L) check[i + 1] = 1; // 1は地雷がある、０はない、２は未確認
        }
    }
    rep(i, L) { // ０あったら確定なので
        if (s[i] == '0') {
            check[i] = 0;
            if (i > 0) check[i - 1] = 0;
            if (i < L) check[i + 1] = 0;
        }
    }

    rep(i, L) { // 1について
        if (s[i] == '1') {
            if (i == 0) { // 0とかn-1の時の処理
                if (check[i] + check[i + 1] == 1) {
                    if (check[0] = 1)
                        check[i + 1] = 0;
                    else
                        check[1] = 0;
                }
                continue;
            } else if (i == L - 1) {
                if (check[i] + check[i - 1] == 1) {
                    if (check[L - 1] = 1)
                        check[L - 2] = 0;
                    else
                        check[L - 2] = 0;
                }
                continue;
            }
            if (check[i] + check[i - 1] + check[i + 1] ==
                1) { // 1があったらそのほか０にする
                if (check[i] == 1) {
                    check[i + 1] = 0;
                    check[i - 1] = 0;
                } else if (check[i - 1] == 1) {
                    check[i]     = 0;
                    check[i + 1] = 0;
                } else {
                    check[i - 1] = 0;
                    check[i]     = 0;
                }
            }
        }
    }
    rep(i, L) {
        if (s[i] == '2') {
            if (i == 0) { // 0とかn-1の時の処理
                check[0] = 1;
                check[1] = 1;
                continue;
            } else if (i == L - 1) {
                check[L - 1] = 1;
                check[L - 2] = 1;
                continue;
            }

            if (check[i - 1] == 0 || check[i] == 0 || check[i] == 0) {
                if(check[i-1] == 0){
                    check[i]=1;
                    check[i+1]=1;
                }else if(check[i] == 0){
                    check[i-1]=1;
                    check[i+1]=1;
                }else{
                    check[i-1]=1;
                    check[i]=1;
                }
            }
        }


    }N