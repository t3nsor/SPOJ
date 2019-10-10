#include<iostream>
using namespace std;

int f(int x, int y){
    if(x == y){
        if (x&1){
            return 2*x-1;
        } 
        else{
            return 2*x;
        }
    }
    else if(x==2+y){
        if(x%2==0){
            return x+y;
        }
        else{
            return x+y-1;
        }
    }
    else{
        return -1;
    }
}

int main(){
    int n,x,y,r;
    cin >> n;
    for(int i=0; i<n; i++){
        cin >> x >> y;
        r = f(x,y);
        if(r==-1){
            cout << "No Number\n";
        }
        else{
            cout << r << endl;
        }
    }
} 