// 2008-02-02
#include <stdio.h>
int main()
{
	int a[10001];
	int t,i,M,F,E,N,j,k;
	int P[500];
	int W[500];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d %d",&E,&F);
		M=F-E;
		scanf("%d",&N);
		for (j=0; j<N; j++)
			scanf("%d %d",&P[j],&W[j]);
		for (j=1; j<=M; j++)
			a[j]=-1;
		a[0]=0;
		for (j=1; j<=M; j++)
		{
			int min=2000000000;
			for (k=0; k<N; k++)
			{
				if (j<W[k]) continue;
				if (a[j-W[k]]==-1) continue;
				if (min>a[j-W[k]]+P[k])
					min=a[j-W[k]]+P[k];
			}
			if (min==2000000000)
				a[j]=-1;
			else
				a[j]=min;
		}
		if (a[M]==-1)
			printf("This is impossible.\n");
		else
			printf("The minimum amount of money in the piggy-bank is %d.\n",a[M]);
	}
	return 0;
}
