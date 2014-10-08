// 2008-06-10
#include <cstdio>
#define MIN(x,y) ((x)<(y)?(x):(y))
double dist[100000];
double K;
int N;
bool f(double T)
{
	double d=dist[0]+T;
	int i;
	for (i=1; i<N; i++)
	{
		if (dist[i]-T>d+K)
			return false;
		d=MIN(d+K,dist[i]+T);
	}
	return true;
}
int main()
{
	int i,t,j;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%lf %d",&K,&N);
		for (j=0; j<N; j++)
			scanf("%lf",&dist[j]);
		double TLow=0.0;
		double THigh=1000000000.0;
		double TMid;
		while (THigh-TLow>0.005)
		{
			TMid=(THigh+TLow)/2.0;
			if (f(TMid))
				THigh=TMid;
			else
				TLow=TMid;
		}
		printf("%.2lf\n",TMid);
	}
	return 0;
}
