// 2010-01-30
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	for (;;)
	{
		double l,w;
		scanf("%lf %lf",&l,&w);
		if (l==0&&w==0) return 0;
		if (l<w) swap(l,w);
		double res=0.0;
		//case 1
		if (l>=w*(1+M_PI))
			res=max(res,M_PI*(w/2.0)*(w/2.0)*w);
		else
		{
			double r=l/(2.0+2.0*M_PI);
			res=max(res,M_PI*r*r*w);
		}
		//case 2
		double r=w/(2.0*M_PI);
		res=max(res,M_PI*r*r*(l-2.0*r));
		printf("%.3f\n",res);
	}
}
