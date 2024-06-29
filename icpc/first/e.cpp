#include <iostream>
using namespace std;
#include <vector>
int main()
{
    vector<bool> A(1000000000);
    for (int i = 0; i < 1000000000; i++)
    {
        A[i] = true;
    }
    A[0] = false;
    A[1] = false;
    for (int k = 2; k < 1000000000; k++)
    {
        if (!A[k])
            continue;
        for (int s = 2; s * k< 100000000 ; s ++)
        {
            A[k * s] = false;
        }
    }
    int count = 0;
    vector<int> p;
    for (int t = 0; t < 1000000000; t++)
    {
        if (A[t] == false)
            continue;
        p.push_back(t);
        count++;
    }
    int N;
    cin >> N;

    int check1 = 0;
    int check2 = 0;
    for (int i = 0; i <= count - 1; i++)
    {
        if (N % p[i] == 0)
        {
            if ((N % p[i]) % p[i] == 0)
            {
                check1++;
                cout << p[i] << endl;
            }
            else
            {
                check2++;
                cout << p[i] << endl;
            }
        }
    }
    if (check1 == 1 && check2 == 1)
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}