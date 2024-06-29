#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)

int ans(int N, int M, ofstream &outputfile) {
    vector<int> aa(N);
    vector<int> bb(M);
    int a, b;
    for (int i = 0; i < N; i++) {
        cin >> a;
        aa[i] = a;
    }
    for (int i = 0; i < M; i++) {
        cin >> b;
        bb[i] = b;
    }
    vector<int> cc(10, 0);
    int c;
    for (int i = 0; i < M; i++) {
        for (int n = 0; n < N; n++) {
            c = aa[n] * bb[i] * 10;
            for (int j = 0; j < 10; j++) {
                c = c / 10;
                if (c == 0) {
                    break;
                } else {
                    cc[c % 10] += 1;
                }
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        if (i != 0) outputfile << ' ';
        outputfile << cc[i] ;
    }
    return 0;
}

int main() {
    ofstream outputfile("ans4.txt");

    int N, M;
    while (cin >> N >> M && N != 0 && M != 0) {
        ans(N, M, outputfile);
        outputfile << endl;
    }

    outputfile.close();
    return 0;
}
