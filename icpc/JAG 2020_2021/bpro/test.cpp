#include <iostream>
#include <string>

using namespace std;

int main() {
    string a = "Hello, World!";
    string d;
    for(char &c : a) {
        cout << c << endl;
        d.push_back(c);
    }
    cout << endl;
    return 0;
}
