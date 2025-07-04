#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for (int i = 0; i < (n); ++i)

int solve(vector<int> a) {
  int n = a.size();
  vector dp(n+1, vector<int>(2));
  rep(i,n) {
    dp[i+1][1] = max(dp[i][0], dp[i][1]);
    dp[i+1][0] = dp[i][1]+a[i];
  }
  int mx = max(dp[n][0], dp[n][1]);
  int sum = 0;
  rep(i,n) sum += a[i];
  return sum-mx;
}

int main() {
  int n, d;
  cin >> n >> d;

  const int M = 1000005;
  vector<int> cnt(M);
  rep(i,n) {
    int a;
    cin >> a;
    cnt[a]++;
  }

  int ans = 0;
  if (d == 0) {
    rep(i,M) {
      if (cnt[i] > 0) ans += cnt[i]-1;
    }
  } else {
    rep(si,d) {
      vector<int> a;
      for (int i = si; i < M; i += d) {
        a.push_back(cnt[i]);
      }
      ans += solve(a);
    }
  }
  cout << ans << endl;
  return 0;
}