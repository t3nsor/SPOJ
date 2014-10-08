// 2008-07-24
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int main()
{
	int T,i,j,S,C;
	int d[100][100];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d %d",&S,&C);
		for (i=0; i<S; i++)
			scanf("%d",d[0]+i);
		for (i=1; i<S; i++)
			for (j=0; j<S-i; j++)
				d[i][j]=d[i-1][j+1]-d[i-1][j];
		for (i=1; i<=C; i++)
			d[S-1][i]=d[S-1][0];
		for (i=S-2; i>=0; i--)
			for (j=S-i; j<S-i+C; j++)
				d[i][j]=d[i+1][j-1]+d[i][j-1];
		for (i=S; i<S+C-1; i++)
			printf("%d ",d[0][i]);
		printf("%d\n",d[0][S+C-1]);
	}
	return 0;
}
