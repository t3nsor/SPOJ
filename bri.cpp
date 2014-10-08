// 2010-11-15
#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;
int main()
{
	int T;
	scanf("%d",&T);
	while (T--)
	{
		int a0,a1,c,h1,s0,s1,i;
		scanf("%d %d",&a0,&a1);
		scanf("%d",&c);
		scanf("%d",&h1);
		scanf("%d %d",&s0,&s1);
		double low=0.0;
		double high=acos(0.0)-(1e-9);
		while (high-low>1e-10)
		{
			double mid=(low+high)/2.0;
			double dx=0;
			double ang=mid;
			double phi;
			double SIN=sin(ang);
			double TAN=SIN/sqrt(1-SIN*SIN);
			dx+=a0*TAN;
			bool critical=false;
			if (SIN*s0/s1>1.0)
				critical=true;
			else
			{
				phi=asin(SIN*s0/s1);
				double SINPHI=SIN*s0/s1;
				dx+=h1*SINPHI/sqrt(1-SINPHI*SINPHI);
				if (SINPHI*s1/s0>1.0)
					critical=true;
				else
					dx+=a1*TAN;
			}
			if (critical||dx>c)
				high=mid;
			else
				low=mid;
		}
		double cost=0;
		double ang=low;
		cost+=s0*a0/cos(ang);
		ang=asin(sin(ang)*s0/s1);
		cost+=s1*h1/cos(ang);
		ang=asin(sin(ang)*s1/s0);
		cost+=s0*a1/cos(ang);
		printf("%.2f\n",cost);
	}
	return 0;
}
