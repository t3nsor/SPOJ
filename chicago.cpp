// 2008-07-22
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
int main()
{
	long double dist[100][100];
	int i,j,k,V,E;
	int V1,V2,D;
	for(;;)
	{
		scanf("%d %d",&V,&E);
		if (V==0)
			return 0;
		for (i=0; i<V; i++)
			for (j=0; j<V; j++)
				dist[i][j]=1000000000.0;
		for (i=0; i<E; i++)
		{
			scanf("%d %d %d",&V1,&V2,&D);
			V1--,V2--;
			dist[V1][V2]=dist[V2][V1]=-log(D/100.0);
		}
		for (k=0; k<V; k++)
			for (i=0; i<V; i++)
				for (j=0; j<V; j++)
					if (dist[i][k]+dist[k][j]<dist[i][j])
						dist[i][j]=dist[i][k]+dist[k][j];
		printf("%.6llf percent\n",100.0L*exp(-dist[0][V-1]));
	}
}
