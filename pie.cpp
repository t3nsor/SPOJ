// 2008-07-22
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#define PI 3.1415926535898
int main()
{
	int T,i,N,F,j;
	double A[10000];
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		scanf("%d %d",&N,&F);
		double total=0.0;
		for (j=0; j<N; j++)
		{
			int r;
			scanf("%d",&r);
			A[j]=PI*r*r;
			total+=A[j];
		}
		double l=0.0;
		double u=total/(F+1);
		double m;
		while (u-l>0.0005)
		{
			m=(l+u)/2;
			int __c=0;
			for (j=0; j<N; j++)
				__c+=int(A[j]/m);
			if (__c>=F+1)
				l=m;
			else
				u=m;
		}
		printf("%.3lf\n",l);
	}
	return 0;
}
