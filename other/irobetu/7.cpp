#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;
int main()
{
    int n;
    cin >> n;
    vector<string> s(n);
    string ans;
    rep(i, n) cin >> s[i];

    rep(i, n)
    {
        if (s[i] == 'b' || s[i] == 'c')
        {
            ans.push_back('1');
        }
        else if (s[i] == 'd' || s[i] == 'w')
        {
            ans.push_back('2');
        }
        else if (s[i] == 't' || s[i] == 'j')
        {
            ans.push_back('3');
        }
        else if (s[i] == 'f' || s[i] == 'q')
        {
            ans.push_back('4');
        }
        else if (s[i] == 'l' || s[i] == 'v')
        {
            ans.push_back('5');
        }
        else if (s[i] == 's' || s[i] == 'x')
        {
            ans.push_back('6');
        }
        else if (s[i] == 'p' || s[i] == 'm')
        {
            ans.push_back('7');
        }
        else if (s[i] == 'h' || s[i] == 'k')
        {
            ans.push_back('8');
        }
        else if (s[i] == 'n' || s[i] == 'g')
        {
            ans.push_back('9');
        }
        else if (s[i] == 'z' || s[i] == 'r')
        {
            ans.push_back('0');
        }
        else
        {
            ans.push_back(' ');
        }
    }
    cout << ans << endl;
}