#include <bits/stdc++.h>
using namespace std;

int main() {
    random_device rd;
    mt19937 rng(rd());

    int t = 10000; // テストケースの数
    cout << t << '\n';

    for (int ti = 0; ti < t; ++ti) {
        int n = rng() % 10 + 1; // nは1～5
        cout << n << '\n';

        vector<int> vals;
        for (int i = 0; i < n; ++i) {
            vals.push_back(i);
            vals.push_back(i);
        }

        shuffle(vals.begin(), vals.end(), rng);
        for (int v : vals) cout << v + 1 << ' ';
        cout << '\n';
    }

    return 0;
}
