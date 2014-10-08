// 2009-06-21
#include <iostream>
#include <cmath>
using namespace std;
const double pi=acos(-1.0);
int main()
{
	int x;
	for(;;)
	{
		scanf("%d",&x);
		if (!x) return 0;
		printf("%.2f\n",x*x/(2.0*pi));
	}
}
