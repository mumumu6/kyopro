#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using ll = long long;

// 1 以上 N 以下の整数が素数かどうかを返す
vector<bool> Eratosthenes(int N)
{
    // テーブル
    vector<bool> isprime(N + 1, true);

    // 1 は予めふるい落としておく
    isprime[1] = false;

    // ふるい
    for (int p = 2; p <= N; ++p)
    {
        // すでに合成数であるものはスキップする
        if (!isprime[p])
            continue;

        // p 以外の p の倍数から素数ラベルを剥奪
        for (int q = p * 2; q <= N; q += p)
        {
            isprime[q] = false;
        }
    }

    // 1 以上 N 以下の整数が素数かどうか
    return isprime;
}

int main()
{
    vector<bool> isprime = Eratosthenes(1000000000);
    vector<int> p;
    int count = 0;
    for (int t = 0; t < 1000000000; t++)
    {
        if (!isprime[t])
            continue;
        p.push_back(t);
      
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

