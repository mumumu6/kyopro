#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./compare file1.txt file2.txt\n";
        return 1;
    }

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    if (!file1.is_open() || !file2.is_open()) {
        cerr << "Error: Could not open one of the files.\n";
        return 1;
    }

    string line1, line2;
    int line_num = 1;
    bool all_match = true;

    while (true) {
        bool f1 = static_cast<bool>(getline(file1, line1));
        bool f2 = static_cast<bool>(getline(file2, line2));

        if (!f1 && !f2) break;  // 両方EOF
        if (line1 != line2) {
            cout << "Difference at line " << line_num << ":\n";
            cout << "  file1: " << line1 << '\n';
            cout << "  file2: " << line2 << '\n';
            all_match = false;
        }

        ++line_num;
    }

    if (all_match) {
        cout << "Files match exactly.\n";
    }

    return 0;
}
