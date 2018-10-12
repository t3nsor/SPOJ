#include <iostream>
#include <stdio.h>
#include <string.h>
 
using namespace std;
 
int dp[205][205][2];
int a[205][205];
char trees[205][205];
 
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int m,n,i,j;
        scanf("%d%d",&m,&n);
        for(i=0;i<m;i++)
        {
            scanf("%s",trees[i]);
            for(j=0;j<n;j++)
            {
                if(trees[i][j]=='T')
                    a[i+1][j+1]=2;
                else if(trees[i][j]=='0')
                    a[i+1][j+1]=1;
                else
                    a[i+1][j+1]=0;
            }
        }
        memset(dp,0,sizeof(dp));
        for(i=m;i>0;i--)
        {
            for(j=1;j<=n;j++)
            {
                if(a[i][j])
                    dp[i][j][0]=max(dp[i][j-1][0],dp[i+1][j][1])+a[i][j]-1;         //0 for right and 1 for left...
            }
            for(j=n;j>0;j--)
            {
                if(a[i][j])
                    dp[i][j][1]=max(dp[i][j+1][1],dp[i+1][j][0])+a[i][j]-1;
            }
        }
        printf("%d\n",dp[1][1][1]);
    }
    return 0;
}
