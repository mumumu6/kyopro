#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
#define rep(i, n) for(int i=0; i<(int)(n); i++) 
#define reps(i,a,n) for(int i=(a); i < (b); i++)

int main(){
    ll sx,sy,tx,ty;
    cin >> sx >> sy >> tx >> ty;
    cout << fixed << setprecision(50);
    if((sx + sy) % 2 == 1)sx -=1;
     if((tx + ty) % 2 == 1)tx -=1;
    ll y = abs(ty-sy);
    ll x = abs(sx - tx);
    
    if(y > x){
        cout << y << endl;
        return 0;
    }else{
        cout << (x-y)/2 + y << endl;

    }


    
}