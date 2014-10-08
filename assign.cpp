// 2009-05-05
#include <iostream>
using namespace std;
int main()
{
	int T,i,j,N;
	int likes[20][20];
	long long dp[1<<20];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		for (i=0; i<N; i++)
			for (j=0; j<N; j++)
				scanf("%d",likes[i]+j);
		dp[0]=1;
		for (i=1; i<(1<<N); i++)
		{
			dp[i]=0;
			int pop=0;
			for (j=0; j<N; j++)
				if (i&(1<<j)) pop++;
			for (j=0; j<N; j++)
				if (i&(1<<j)&&likes[pop-1][j])
					dp[i]+=dp[i-(1<<j)];
		}
		printf("%lld\n",dp[(1<<N)-1]);
	}
	return 0;
}
