// 2008-07-24
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int main()
{
	int t,i,j,N;
	int dp[201];
	char r[200];
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d\n",&N);
		dp[0]=0;
		for (i=1; i<=N; i++)
		{
			scanf("%c",&r[i-1]);
			dp[i]=0;
			int sour=0;
			int sweet=0;
			for (j=1; j<=i; j++)
			{
				if (r[i-j]-'0')
					sweet++;
				else
					sour++;
				if (dp[i]<dp[i-j]+(sweet>sour?j:0))
					dp[i]=dp[i-j]+(sweet>sour?j:0);
			}
		}
		printf("%d\n",dp[N]);
	}
	return 0;
}
