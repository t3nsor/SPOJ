// 2010-11-15
#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;
long double integral(long double h,long double w,long double f,long double x)
{
	return -(w-2*x)*sqrt((w*w*w*w+16*h*h*(w-2*x)*(w-2*x))/(w*w*w*w))/4
		-(w*w*asinh(4*h*(w-2*x)/(w*w)))/(16*h);
}
long double objective(long double h,long double w,long double f,long double d)
{
	return 2*(f*d+integral(h,w,f,w/2)-integral(h,w,f,d));
}
int main()
{
	for (;;)
	{
		long double h,w,f;
		scanf("%Lf %Lf %Lf",&h,&w,&f);
		if (h==0) return 0;
		long double a=64*(h*h);
		long double b=-64*(h*h)*w;
		long double c=16*(h*h)*(w*w)+(w*w*w*w)-(f*f)*(w*w*w*w);
//		printf("a=%Lf b=%Lf c=%Lf\n",a,b,c);
		long double disc=b*b-4*a*c;
		long double x1,x2;
		if (disc<0) x1=x2=-1;
		if (disc>=0)
		{
			x1=(-b+sqrt(b*b-4*a*c))/(2*a);
			x2=(-b-sqrt(b*b-4*a*c))/(2*a);
		}
//		printf("x1=%Lf x2=%Lf\n",x1,x2);
		pair<long double,long double> res;
		res.first=1e200;
		res=min(res,make_pair(objective(h,w,f,0),0.0L));
		res=min(res,make_pair(objective(h,w,f,w/2),w/2));
		if (x1>=0&&x1<=w/2)
			res=min(res,make_pair(objective(h,w,f,x1),x1));
		if (x2>=0&&x2<=w/2)
			res=min(res,make_pair(objective(h,w,f,x2),x2));
//		printf("%.3Lf\n",res.second);
		printf("%.3Lf\n",4*h/(w*w)*(res.second-w/2)*(res.second-w/2)+1e-15);
	}
}

