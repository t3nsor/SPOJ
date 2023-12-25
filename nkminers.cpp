// 2008-04-17
// updated 2023-12-25 for legibility
#include <algorithm>
#include <stdio.h>
#include <string.h>
using namespace std;
void update(int& x, int y) {
    if (x < y) x = y;
}
int main() {
    int N;
    scanf("%d",&N);
    int dp[4][4][4][4]; //how many before:two before to 1:previous to 1:two before to 2:previous to 2
    int dp2[4][4][4][4];
    int i,j,k,m,n,p,q,x;
    char c;
    memset(dp,-100,sizeof(dp));
    dp[0][0][0][0]=0;
    c=getchar();
    for (i=0; i<N; i++) {
        c=getchar();
        if (c=='M') {
            x=1;
        } else if (c=='F') {
            x=2;
        } else {
            //C=='B'
            x=3;
        }
        memset(dp2,-100,sizeof(dp));
        for (k=0; k<4; k++) {
            for (m=0; m<4; m++) {
                for (p=0; p<4; p++) {
                    for (q=0; q<4; q++) {
                        //no food yet
                        if (m==0) {
                            update(dp2[0][x][p][q],dp[0][0][p][q]+1);
                        } else if (k==0) {
                            //one food shipment to mine 1
                            if (x==m) {
                                update(dp2[m][x][p][q],dp[0][m][p][q]+1);
                            } else {
                                update(dp2[m][x][p][q],dp[0][m][p][q]+2);
                            }
                        } else {
                            if (x==k&&x==m) {
                                update(dp2[m][x][p][q],dp[k][m][p][q]+1);
                            } else if (x==k||x==m||m==k) {
                                update(dp2[m][x][p][q],dp[k][m][p][q]+2);
                            } else {
                                update(dp2[m][x][p][q],dp[k][m][p][q]+3);
                            }
                        }
                        if (q==0) {
                            //none yet
                            update(dp2[k][m][0][x],dp[k][m][0][0]+1);
                        } else if (p==0) {
                            //one food shipment to mine 2
                            if (x==q) {
                                update(dp2[k][m][q][x],dp[k][m][0][q]+1);
                            } else {
                                update(dp2[k][m][q][x],dp[k][m][0][q]+2);
                            }
                        } else {
                            if (x==p&&x==q) {
                                update(dp2[k][m][q][x],dp[k][m][p][q]+1);
                            } else if (x==p||x==q||p==q) {
                                update(dp2[k][m][q][x],dp[k][m][p][q]+2);
                            } else {
                                update(dp2[k][m][q][x],dp[k][m][p][q]+3);
                            }
                        }
                    }
                }
            }
        }
        memcpy(dp,dp2,sizeof(dp));
    }
    int best=0;
    for (k=0; k<4; k++) {
        for (m=0; m<4; m++) {
            for (p=0; p<4; p++) {
                for (q=0; q<4; q++) {
                    update(best,dp[k][m][p][q]);
                }
            }
        }
    }
    printf("%d\n",best);
    return 0;
}
