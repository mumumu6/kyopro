#include<iostream>
#include <vector>

using namespace std;

int main() {
    int n,k;
    cin >> n >> k;
    vector<int>a;
    vector<int>b(n);
    vector<int>count;
    for (int i=0 ; i <= 100 ; i++ ){
        count.push_back(i);
    }
    for (int g=0 ; g < k ; g++ ){
        int x;
        cin >> x;
        a.push_back(x);
    }
    for (int h =0; h < k ; h++){
        if( a[h] > 0){
            b[h]=a[h];
            int e =b[h];
            count[e]++;

        }else{
            int c=k;
            int d;
            for (int j=0 ; j < n; j++){
                if(c>count[j]){
                    c=count[j];
                    d=j;
                }
            }
            b[h]=a[d];
            int f = b[h];
            count[f]++;

        }
        cout << b[h] << " ";
    }


}