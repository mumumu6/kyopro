#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for(int i=0;i<(n);++i)
#define el '\n'

/* ----------------------------------------------------
   方向ベクトルと id⇔(y,x) 変換ヘルパ
---------------------------------------------------- */
const int DY[4]={-1,1,0,0};
const int DX[4]={0,0,-1,1};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m; cin>>n>>m;
    vector<vector<char>> s(n,vector<char>(n));
    rep(i,n)rep(j,n) cin>>s[i][j];

    auto idx=[&](int y,int x){ return y*n+x; };

    /* ---------- 重み (整数) と nxt[4] ----------- */
    vector<long long> q(n*n,1), buf(n*n,0);
    rep(i,n)rep(j,n) if(s[i][j]=='#') q[idx(i,j)]=0;

    vector<int> nxt[4]; rep(d,4) nxt[d].assign(n*n,0);

    auto build_nxt=[&](){
        /* 列 ↑↓ */
        rep(x,n){
            int last=-1;
            rep(y,n){
                int id=idx(y,x);
                if(s[y][x]=='#'){ last=y; continue; }
                nxt[0][id]=idx(last+1,x);
            }
            last=n;
            for(int y=n-1;y>=0;--y){
                int id=idx(y,x);
                if(s[y][x]=='#'){ last=y; continue; }
                nxt[1][id]=idx(last-1,x);
            }
        }
        /* 行 ←→ */
        rep(y,n){
            int last=-1;
            rep(x,n){
                int id=idx(y,x);
                if(s[y][x]=='#'){ last=x; continue; }
                nxt[2][id]=idx(y,last+1);
            }
            last=n;
            for(int x=n-1;x>=0;--x){
                int id=idx(y,x);
                if(s[y][x]=='#'){ last=x; continue; }
                nxt[3][id]=idx(y,last-1);
            }
        }
    };
    build_nxt();

    auto patch_nxt=[&](int y,int x){
        int last=-1;
        rep(i,n){
            int id=idx(i,x);
            if(s[i][x]=='#'){ last=i; continue; }
            nxt[0][id]=idx(max(last+1,0),x);
        }
        last=n;
        for(int i=n-1;i>=0;--i){
            int id=idx(i,x);
            if(s[i][x]=='#'){ last=i; continue; }
            nxt[1][id]=idx(min(last-1,n-1),x);
        }
        last=-1;
        rep(j,n){
            int id=idx(y,j);
            if(s[y][j]=='#'){ last=j; continue; }
            nxt[2][id]=idx(y,max(last+1,0));
        }
        last=n;
        for(int j=n-1;j>=0;--j){
            int id=idx(y,j);
            if(s[y][j]=='#'){ last=j; continue; }
            nxt[3][id]=idx(y,min(last-1,n-1));
        }
    };

    /* ---------- 分布を1手進め (整数→整数) ---------- */
    auto step_prob=[&](){
        fill(buf.begin(),buf.end(),0LL);
        rep(id,n*n){
            if(s[id/n][id%n]=='#') continue;
            long long w=q[id];
            if(!w) continue;
            long long share=(w+3)>>2;      // (w+3)/4
            rep(d,4) buf[ nxt[d][id] ] += share;
        }
        q.swap(buf);
    };

    /* ---------- k=2 手 look-ahead で列 P を構築 ---------- */
    int remain=n*n-m;
    vector<double> v0(n*n), v1(n*n), v2(n*n);
    rep(turn,remain){
        /* --- 0,1,2 手先の確率分布を作る（double） --- */
        rep(id,n*n) v0[id]=(s[id/n][id%n]=='#')?0.0:static_cast<double>(q[id]);

        fill(v1.begin(),v1.end(),0.0);
        rep(id,n*n){
            if(v0[id]==0.0) continue;
            double share=v0[id]*0.25;
            rep(d,4) v1[ nxt[d][id] ] += share;
        }

        fill(v2.begin(),v2.end(),0.0);
        rep(id,n*n){
            if(v1[id]==0.0) continue;
            double share=v1[id]*0.25;
            rep(d,4) v2[ nxt[d][id] ] += share;
        }

        /* --- F(i)=v0+v1+v2 が最小のマス best --- */
        int best=-1;
        double bestF=1e300;
        rep(id,n*n){
            if(s[id/n][id%n]=='#') continue;
            double f=v0[id]+v1[id]+v2[id];
            if(f<bestF){
                bestF=f; best=id;
            }
        }
        int y=best/n, x=best%n;
        cout<<y<<" "<<x<<el;

        /* --- 岩を置き & 更新 --- */
        s[y][x]='#';
        q[best]=0;
        patch_nxt(y,x);
        step_prob();
    }
}