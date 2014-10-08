// 2008-07-21
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#define MIN(x,y) ((x)<(y)?(x):(y))
using namespace std;
int main()
{
	int t,i,m,n,j,k;
	int a[1000];
	int b[1000];
	static int cost[1000][1000];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d %d",&m,&n);
		m--,n--;
		for (j=0; j<m; j++)
			scanf("%d",a+j);
		for (j=0; j<n; j++)
			scanf("%d",b+j);
		sort(a,a+m);
		reverse(a,a+m);
		sort(b,b+n);
		reverse(b,b+n);
		cost[0][0]=0; //# of breaks made on a, # of breaks made on b
		for (j=1; j<=m; j++)
			cost[j][0]=cost[j-1][0]+a[j-1];
		for (j=1; j<=n; j++)
			cost[0][j]=cost[0][j-1]+b[j-1];
		for (j=1; j<=m; j++)
			for (k=1; k<=n; k++)
				cost[j][k]=MIN(cost[j-1][k]+(k+1)*a[j-1],cost[j][k-1]+(j+1)*b[k-1]);
		printf("%d\n",cost[m][n]);
	}
	return 0;
}
