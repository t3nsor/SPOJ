// 2008-08-02
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int main()
{
/*#ifndef ONLINE_JUDGE
	freopen("musket.in","r",stdin);
	freopen("musket.out","w",stdout);
#endif*/
	int T,N,i,j,k;
	bool dp[105][105];
	int matrix[105][105];
	bool win[105];
	char s[105];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		for (j=0; j<N; j++)
		{
			scanf("%s",&s);
			for (i=0; i<N; i++)
				matrix[j][i]=s[i]-'0';
		}
		for (i=0; i<N; i++)
			dp[i][(i+1)%N]=true;
		for (j=2; j<=N; j++)
			for (i=0; i<N; i++)
			{
				dp[i][(i+j)%N]=false;
				for (k=(i+1)%N; k!=(i+j)%N; k=(k+1)%N)
					if (dp[i][k]&&dp[k][(i+j)%N]&&(matrix[i][k]||matrix[(i+j)%N][k]))
					{
						dp[i][(i+j)%N]=true;
						break;
					}
			}
		int cnt=0;
		for (i=0; i<N; i++)
			if (dp[i][i])
				cnt++;
		printf("%d\n",cnt);
		for (i=0; i<N; i++)
			if (dp[i][i])
				printf("%d\n",i+1);
	}
	return 0;
}
