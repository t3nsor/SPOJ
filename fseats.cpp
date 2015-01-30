// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int ok[301][301];
int a[301];
int totalseats[301][301];
int main()
{
	int X,Y,K,i,j,minext,best,beg,end,sum,k;
#ifndef ONLINE_JUDGE
	freopen("find.in","r",stdin);
#endif
	for(;;)
	{
		scanf("%d %d %d",&Y,&X,&K);
		if (!Y) return 0;
		int x=X;
		int y=Y;
		if (Y>X) swap(X,Y);
		for (i=1; i<=y; i++)
		{
			getchar();
			for (j=1; j<=x; j++)
				if (getchar()-'X')
					if (y<x)
						ok[j][i]=1;
					else
						ok[i][j]=1;
				else
					if (y<x)
						ok[j][i]=0;
					else
						ok[i][j]=0;
		}
		for (i=0; i<X; i++)
			totalseats[i][0]=0;
		for (i=0; i<Y ;i++)
			totalseats[0][i]=0;
		for (i=1; i<=X; i++)
			for (j=1; j<=Y; j++)
				totalseats[i][j]=ok[i][j]+totalseats[i-1][j]+totalseats[i][j-1]-totalseats[i-1][j-1];
		minext=1000000000;
		for (i=1; i<=Y; i++)
			for (j=i; j<=Y; j++)
			{
				if (K>totalseats[X][j]-totalseats[X][i-1])
					continue;
				beg=1;
				end=1;
				sum=0;
				best=1000000;
				for(;;)
					if (sum<K)
						if (end<=X)
							sum+=a[end++]=totalseats[end][j]-totalseats[end-1][j]-totalseats[end][i-1]+totalseats[end-1][i-1];
						else
							break;
					else
					{
						best=min(best,end-beg);
						sum-=a[beg++];
					}
				minext=min(minext,best*(j-i+1));
			}
		printf("%d\n",minext);
	}
	return 0;
}
