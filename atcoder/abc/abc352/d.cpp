#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;
    vector<int> q(n);
    for (int i = 0; i < n; i++)
    {
        int p;
        cin >> p;
        --p;
        q[p] = i;
    }
    set<int> st;
    for (int i = 0; i < k; i++)
    {
        st.insert(q[i]);
    }
    // *st.rbegin() : 最大値の取得
    // *st.begin() : 最小値の取得
    int ans = *st.rbegin() - *st.begin();
    for (int i = k; i < n; i++)
    {
        st.erase(q[i - k]);
        st.insert(q[i]);
        ans = min(ans, *st.rbegin() - *st.begin());
    }
    cout << ans << endl;
}
