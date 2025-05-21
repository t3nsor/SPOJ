// 2010-07-01
// updated 2025-05-20 for SPOJ version
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;
vector<int> L[50000];
vector<int> F[50000];
int ok(int c,int bitmask)
{
    int cnt=L[c].size();
    int i;
    for (i=0; i<L[c].size(); i++)
	if (bitmask&(1<<L[c][i]))
	    cnt--;
    if (cnt) return 1;
    cnt=0;
    for (i=0; i<F[c].size(); i++)
	if (bitmask&(1<<F[c][i]))
	    cnt++;
    return !!cnt;
}
int main()
{
    int T; scanf("%d", &T);
    while (T--) {
        int N,C,x,f,l,i,j,k,m,n,cnt,res;
        int E[50000];
        int dp[10000][16][16]={0};
        scanf("%d %d",&N,&C);
        for (i=0; i<C; i++)
        {
            F[i].clear();
            L[i].clear();
            scanf("%d",E+i);
            E[i]--;
            scanf("%d %d",&f,&l);
            for (j=0; j<f; j++)
            {
                scanf("%d",&x); x--;
                F[i].push_back((x-E[i]+N)%N);
            }
            for (j=0; j<l; j++)
            {
                scanf("%d",&x); x--;
                L[i].push_back((x-E[i]+N)%N);
            }
        }
        j=0;
        while (j<C&&E[j]<=3) j++;
        //brute force first 8
        for (i=0; i<256; i++)
        {
            dp[7][i/16][i%16]=0;
            for (k=0; k<j; k++)
                    dp[7][i/16][i%16]+=ok(k,31&(i>>E[k]));
        }
        //DP
        for (i=8; i<N; i++)
        {
            k=j;
            while (j<C&&E[j]<=i-4) j++;
            for (m=0; m<32; m++) //last five entries
            {
                cnt=0;
                for (n=k; n<j; n++)
                    cnt+=ok(n,m);
                for (n=0; n<16; n++) //first 4 bits
                    dp[i][m/2][n]=max(dp[i][m/2][n],cnt+dp[i-1][m&15][n]);
            }
        }
        res=0;
        //wrap-arounds
        for (i=0; i<16; i++)
            for (k=0; k<16; k++) //we have to correct dp[N-1][i][k]
            {
                for (m=j; m<C; m++) //last few kids
                    dp[N-1][i][k]+=ok(m,(i>>(E[m]-N+4))+((k%(1<<(E[m]-N+5)))<<(N-E[m])));
                res=max(res,dp[N-1][i][k]);
            }
        printf("%d\n",res);
    }
    return 0;
}
