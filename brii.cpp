// 2010-11-15
#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;
int main()
{
	int T,i,N,c,a[100],h[100],s[100];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		for (i=0; i<=N; i++)
			scanf("%d",a+i);
		for (i=1; i<=N; i++)
			scanf("%d",h+i);
		scanf("%d",&c);
		for (i=0; i<=N; i++)
			scanf("%d",s+i);
		double low=0.0;
		double high=acos(0.0)-(1e-9);
		while (high-low>1e-12)
		{
			double mid=(low+high)/2.0;
			double dx=0;
			double ang=mid;
			dx+=a[0]*tan(ang);
			bool critical=false;
			for (i=1; i<=N; i++)
			{
				//sin(ang)*n1 = sin(new_ang)*n2
				//new_ang = asin(sin(ang)*n1/n2))
				if (sin(ang)*s[0]/s[i]>1.0)
				{
					critical=true;
					break;
				}
				else
					ang=asin(sin(ang)*s[0]/s[i]);
				dx+=h[i]*tan(ang);
				if (sin(ang)*s[i]/s[0]>1.0)
				{
					critical=true;
					break;
				}
				else
					ang=asin(sin(ang)*s[i]/s[0]);
				dx+=a[i]*tan(ang);
			}
			if (critical||dx>c)
				high=mid;
			else
				low=mid;
		}
		double cost=0;
		double ang=low;
		cost+=s[0]*a[0]/cos(ang);
		for (i=1; i<=N; i++)
		{
			//sin(ang)*n1 = sin(new_ang)*n2
			//new_ang = asin(sin(ang)*n1/n2))
			ang=asin(sin(ang)*s[0]/s[i]);
			cost+=s[i]*h[i]/cos(ang);
			ang=asin(sin(ang)*s[i]/s[0]);
			cost+=s[0]*a[i]/cos(ang);
		}
		printf("%.2f\n",cost);
	}
	return 0;
}

