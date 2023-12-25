// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	int t,a;
	double r;
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d",&a);
		r=pow(3.0*a/(516.0*3.1415926536),1.0/3);
		printf("%d\n",int(18.0*r));
	}
	return 0;
}
