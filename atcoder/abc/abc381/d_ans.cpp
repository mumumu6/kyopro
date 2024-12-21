#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for(int i = 0; i < n; ++i)
#define N 200000

int main() {
	int n,l,ans=0;
	int a[N];
	int last[N+1];
	cin>>n;
	rep(i,n)cin>>a[i];

	rep(i,N)last[i+1]=-2;
	l=0;
	for(int i=0;(i+1)<n;i+=2){
		if(a[i]!=a[i+1])l=i+2;
		else l=max(l,last[a[i]]+2);
		ans=max(ans,i+2-l);
		last[a[i]]=i;
	}

	rep(i,N)last[i+1]=-2;
	l=1;
	for(int i=1;(i+1)<n;i+=2){
		if(a[i]!=a[i+1])l=i+2;
		else l=max(l,last[a[i]]+2);
		ans=max(ans,i+2-l);
		last[a[i]]=i;
	}
	cout<<ans<<endl;
	return 0;
}