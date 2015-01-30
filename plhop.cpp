// 2008-09-16
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int T,_i,K,N,i,j,k,x;
	long long a[50][50];
	long long A[50][50];
	long long t[50][50];
	scanf("%d",&T);
	for (_i=1; _i<=T; _i++)
	{
		scanf("%d",&K);
		scanf("%d",&N);
		for (i=0; i<N; i++)
			for (j=0; j<N; j++)
			{
				scanf("%lld",a[i]+j);
				A[i][j]=a[i][j];
			}
		x=1;
		while (x<=K) x<<=1; x>>=1;
		while (x>>=1)
		{
			for (i=0; i<N; i++)
				for (j=0; j<N; j++)
				{
					t[i][j]=(long long)1e15;
					for (k=0; k<N; k++)
						t[i][j]=min(t[i][j],A[i][k]+A[k][j]);
				}
			if (K&x)
				for (i=0; i<N; i++)
					for (j=0; j<N; j++)
					{
						A[i][j]=(long long)1e15;
						for (k=0; k<N; k++)
							A[i][j]=min(A[i][j],t[i][k]+a[k][j]);
					}
			else
				memcpy(A,t,sizeof(A));
		}
		printf("Region #%d:\n",_i);
		for (i=0; i<N; i++)
		{
			for (j=0; j<N-1; j++)
				printf("%lld ",A[i][j]);
			printf("%lld\n",A[i][N-1]);
		}
		putchar('\n');
	}
	return 0;
}
