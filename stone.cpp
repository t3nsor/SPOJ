// 2008-06-26
// modified 2014-10-07
#include <stdio.h>
typedef long long i64;
int main()
{
	int T,i,N,j;
	static int X[1000000];
	static int Y[1000000];
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		scanf("%d",&N);
		for (j=0; j<N; j++)
			scanf("%d %d",X+j,Y+j);
		X[N]=X[0],Y[N]=Y[0];
		i64 area=0,x=0,y=0;
		for (j=0; j<N; j++)
		{
			x+=(i64)(X[j]+X[j+1])*(i64)(X[j]*Y[j+1]-X[j+1]*Y[j]);
			y+=(i64)(Y[j]+Y[j+1])*(i64)(X[j]*Y[j+1]-X[j+1]*Y[j]);
			area+=(i64)(X[j]*Y[j+1]-X[j+1]*Y[j]);
		}
		double _x=(long double)x/(3.0l*(long double)area);
		double _y=(long double)y/(3.0l*(long double)area);
		printf("%.2lf %.2lf\n",_x,_y);
	}
	return 0;
}
