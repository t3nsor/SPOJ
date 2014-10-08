// 2009-05-06
#include <iostream>
using namespace std;
int main()
{
	int a[2222];
	int t[2222];
	static int dp[2222][2222];
	int T,x,i,j;
	scanf("%d",&T);
	while (T--)
	{
		int N_a=0;
		for (;;)
		{
			scanf("%d",&x);
			if (!x) break;
			a[N_a++]=x;
		}
		int best=0;
		for (;;)
		{
			int N_t=0;
			for (;;)
			{
				scanf("%d",&x);
				if (!x) break;
				t[N_t++]=x;
			}
			if (!N_t) break;
			for (i=1; i<=N_a; i++)
				for (j=1; j<=N_t; j++)
				{
					dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
					if (a[i-1]==t[j-1])
						dp[i][j]=max(dp[i][j],1+dp[i-1][j-1]);
				}
			best=max(best,dp[N_a][N_t]);
		}
		printf("%d\n",best);
	}
	return 0;
}
