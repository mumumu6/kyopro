#include <bits/stdc++.h>
#include <cstdlib>
#define ll long long
using namespace std;
int main() {
    ofstream outputfile("ans.txt");
    int h, w;
    cin >> h >> w;
    while (h != 0 || w != 0) {
        vector<vector<char>> v(h, vector<char>(w));
        double xg = 0, yg = 0, wx = 0, wy = 0;
        int z = 0;
        double t;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                cin >> v.at(i).at(j);
                z = int(v.at(i).at(j) - '0');
                t = (double)z;
                xg += t * (double)(i + 1);
                wx += t;
                yg += t * (double)(j + 1);
                wy += t;
            }
        }
        outputfile << fixed << setprecision(10);
        outputfile << xg / wx << " " << yg / wy << "\n";
        cin >> h >> w;
    }
}