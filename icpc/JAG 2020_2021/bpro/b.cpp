#include <bits/stdc++.h>
using namespace std;
#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;
using ll = long long;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define reps(i,a,b) for(ll i = (a); i < (b); i++)

 int main(){
    ofstream outputfile("ans4.txt");

    int w,h,n,d,b;
    cin >> w >> h;
    vector<vector<bool>> bom(w,vector<bool>(h,false));
    vector<vector<int>> bombed(d,vector<int>(2,0));
    int cd=0;
    int cr=0;
    int x,y;
    while(w!=0 || h!=0){
        cin>> n >> d >> b;
        for(int i=0; i<n;i++){
            cin >> x >> y;
            x--;
            y--;
            bom[x][y] = true;
            if(i+1==b){
                bombed[0][0]=x;
                bombed[0][1]=y;
                cd=0;
            }
        }
        while(cr!=cd+1){
            x=bombed[cd][0];
            y=bombed[cd][1];
        for(int j=0;j<d;j++){
            if(y>j){
                if(bom[x][y-j-1]){
                    cd++;
                    bombed[cd][0]=x;
                    bombed[cd][1]=y-j-1;
                    bom[x][y-j-1]=false;
                    break;
                }
            } else {
                break;
            }
        }
        for(int k=0;k<d;k++){
            if(x>k){
                if(bom[x-k-1][y]){
                    cd++;
                    bombed[cd][0]=x-k-1;
                    bombed[cd][1]=y;
                    bom[x-k-1][y]=false;
                    break;
                }
            } else {
                break;
            }
        }
        for(int l=0;l<d;l++){
            if(x+l+1<w){
                if(bom[x+l+1][y]){
                    cd++;
                    bombed[cd][0]=x+l+1;
                    bombed[cd][1]=y;
                    bom[x+l+1][y]=false;
                    break;
                }
            }
        }
        for(int m=0;m<d;m++){
            if(y+m+1<h){
                if(bom[x][y+m+1]){
                    cd++;
                    bombed[cd][0]=x;
                    bombed[cd][1]=y+m+1;
                    bom[x][y+m+1]=false;
                    break;
                }
            }
        }
        cd++;
        }
        outputfile <<cr<<"\n";
        cd=0;
        cr=0;
        cin >> w >> h;
    }
    outputfile.close();
 }