// 2009-05-06
#include <iostream>
#include <cmath>
using namespace std;
const double pi=acos(-1.0);
int main()
{
	int a,b,s,m,n;
	for(;;)
	{
		scanf("%d %d %d %d %d",&a,&b,&s,&m,&n);
		if (!a) return 0;
		double horiz=a*m; //inches
		double vert=b*n;
		double angle=atan2(vert,horiz)*180.0/pi;
		double dist=sqrt(horiz*horiz+vert*vert);
		double speed=dist/s;
		printf("%.2f %.2f\n",angle,speed);
	}
	return 0;
}
