// 2008-08-02
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int T,O,N,n,_o,_n,_w,i,j;
	scanf("%d",&T);
	int dp[22][80];
	int tmp[22][80];
	while(T--)
	{
		scanf("%d %d %d",&O,&N,&n);
		memset(dp,0x7F,sizeof(dp));
		dp[0][0]=0;
		while (n--)
		{
			scanf("%d %d %d",&_o,&_n,&_w);
			memcpy(tmp,dp,sizeof(dp));
			for (i=0; i<=O; i++)
				for (j=0; j<=N; j++)
					if (tmp[min(i+_o,O)][min(j+_n,N)]>dp[i][j]+_w)
						tmp[min(i+_o,O)][min(j+_n,N)]=dp[i][j]+_w;
			memcpy(dp,tmp,sizeof(dp));
		}
		printf("%d\n",dp[O][N]);
	}
	return 0;
}
