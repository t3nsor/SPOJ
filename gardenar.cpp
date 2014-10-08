// 2009-12-10
#include <iostream>
#include <cmath>
using namespace std;
double max3(double x,double y,double z)
{
	return max(x,max(y,z));
}
double min3(double x,double y,double z)
{
	return min(x,min(y,z));
}
int main()
{
	int T;
	double x,y,z;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%lf %lf %lf",&x,&y,&z);
		double l=max3(fabs(x-y),fabs(y-z),fabs(z-x));
		double u=min3(x+y,y+z,z+x);
		while (u-l>1e-9)
		{
			double s=(l+u)/2.0;
			double a1=acos((x*x+y*y-s*s)/(2.0*x*y));
			double a2=acos((y*y+z*z-s*s)/(2.0*y*z));
			double a3=acos((z*z+x*x-s*s)/(2.0*z*x));
			if (a1+a2+a3<2.0*acos(-1.0))
				l=s;
			else
				u=s;
		}
		printf("%.2f\n",sqrt(3.0)*l*l/4.0);
	}
	return 0;
}
