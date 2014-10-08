// 2009-04-11
#include <iostream>
#include <cmath>
using namespace std;
double pi=acos(-1.0);
int main()
{
	int T;
	double V0,K1,K2,theta,x,y;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%lf %lf %lf",&V0,&K1,&K2);
		theta=atan(-4.0*K1/K2); //tan theta = -4k1/k2
		while (theta<=0) theta+=pi; //pi!
		theta/=2.0;
		double sintheta=sin(theta);
		double sinsquaredtheta=sintheta*sintheta;
		x=V0*V0*2*sintheta*sqrt(1-sinsquaredtheta)/10.0;
		y=V0*V0*sinsquaredtheta/20.0;
		printf("%.3f %.3f\n",theta,K1*x+K2*y);
	}
	return 0;
}
