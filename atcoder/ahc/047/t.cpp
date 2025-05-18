#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using ll = long long;

#define rep(i,n) for(ll i=0;i<(n);++i)
#define el '\n'

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---- 入力 ---- */
    ll N,M,L;                         // N=36, M=12, L=1e6
    cin>>N>>M>>L;
    vector<pair<ll,string>> v(N);     // {P,S}
    rep(i,N) cin>>v[i].second>>v[i].first;
    sort(v.begin(),v.end(),greater<>()); // 得点降順

    /* ---- 1. 文字列 t は v[0] ＋ 穴埋め ---- */
    string t=v[0].second;
    set<char> used(t.begin(),t.end());
    for(char c='a';c<='f'&&t.size()<M;++c)
        if(!used.count(c)) t.push_back(c);
    while(t.size()<M) t.push_back('a');

    /* ---- 2. 各文字ごとに “最初の状態番号” ---- */
    vector<int> pos(6,-1);            // 'a'→0 … 'f'→5
    rep(i,M){
        int c=t[i]-'a';
        if(pos[c]==-1) pos[c]=i;
    }

    /* ---- 3. 遷移行列 ---- */
    vector<vector<int>> A(M,vector<int>(M,0));

    /* 3-1 主サイクル：40 % */
    rep(i,M){
        A[i][(i+1)%M]=40;             // 次の状態
        A[i][i]      =60;             // 残りは自ループ
    }

    /* ヘルパ：行を “必ず next へ 100 %” に書き換え */
    auto force = [&](int from,int to){
        fill(A[from].begin(),A[from].end(),0);
        A[from][to]=100;
    };

    /* 3-2 2 位・3 位にブランチ */
    for(int rk=1; rk<=2 && rk<N; ++rk){
        const string &s=v[rk].second;
        int d=s.size();
        if(d<2) continue;

        /* パス（状態列）を作る */
        vector<int> path;
        rep(i,d){
            int p=pos[s[i]-'a'];
            if(p==-1){ path.clear(); break; }
            path.push_back(p);
        }
        if(path.empty()) continue;

        /* (a) ヘッド 30 % を割り当てる */
        int f=path[0], to=path[1];
        int take=30;
        // 自ループから移す
        A[f][f]-=take;
        A[f][to]+=take;

        /* (b) ボディは 100 % で固定 */
        for(int i=1;i<d-1;++i){
            force(path[i], path[i+1]);
        }
    }

    /* 3-3 行を 100 % に再調整（不足ぶんだけ自ループ） */
    rep(i,M){
        int sm=0; rep(j,M) sm+=A[i][j];
        if(sm<100) A[i][i]+=100-sm;
    }

    /* ---- 4. 出力 ---- */
    rep(i,M){
        cout<<t[i];
        rep(j,M) cout<<' '<<A[i][j];
        cout<<el;
    }
    return 0;
}
