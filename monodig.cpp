// 2008-09-30
// updated 2023-10-05 to fix compilation errors after rejudging
#include <iostream>
#include <set>
using namespace std;
int main() {
    set<int> ok[10][9]; //digit, length
    int i,j,k;
    set<int>::iterator I,J;
    for (i=1; i<=9; i++) {
        int x=0;
        for (j=1; j<=8; j++) {
            x=10*x+i;
            ok[i][j].insert(x);
        }
        for (j=2; j<=8; j++) {
            for (k=1; k<j; k++) {
                for (I=ok[i][k].begin(); I!=ok[i][k].end(); I++) {
                    for (J=ok[i][j-k].begin(); J!=ok[i][j-k].end(); J++) {
                        ok[i][j].insert(*I + *J);
                        ok[i][j].insert(*I - *J);
                        ok[i][j].insert(*I * *J);
                        if (*J&&*I%*J==0) ok[i][j].insert(*I / *J);
                    }
                }
            }
        }
    }
    int T,K,N,n;
    cin >> T;
    while (T--) {
        cin >> K >> N;
        while (N--) {
            cin >> n;
            bool b=false;
            for (i=1; i<=8; i++) {
                if (ok[K][i].find(n)!=ok[K][i].end()) {
                    b=true;
                    break;
                }
            }
            if (b) cout << i << '\n'; else puts("NO");
        }
    }
    return 0;
}
