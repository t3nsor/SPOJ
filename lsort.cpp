// 2008-07-23
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
inline int min(int x,int y)
{
	return x<y?x:y;
}
int main()
{
	int T,N,i,j;
	scanf("%d",&T);
	static int cost[1001][1001];
	int pos[1001];
	static int pos1[1001][1001];
	static int pos2[1001][1001];
	int x;
	while (T--)
	{
		scanf("%d",&N);
		for (i=1; i<=N; i++)
		{
			scanf("%d",&x);
			pos[x]=i;
		}
		for (i=1; i<=N; i++)
		{
			cost[i][i]=pos[i];
			pos1[i][i]=pos[i];
			pos2[i][i]=pos[i];
		}
		for (i=1; i<N; i++)
			for (j=1; j<=N-i; j++)
			{
				pos1[j][i+j]=pos1[j][i+j-1]-(pos[i+j]<pos[j]?1:0);
				pos2[j][i+j]=pos2[j+1][i+j]-(pos[j]<pos[i+j]?1:0);
				cost[j][i+j]=min(cost[j+1][i+j]+(i+1)*pos1[j][i+j],cost[j][i+j-1]+(i+1)*pos2[j][i+j]);
			}
		printf("%d\n",cost[1][N]);
	}
	return 0;
}
