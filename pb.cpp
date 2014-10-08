// 2009-06-18
#include <iostream>
using namespace std;
long long mulmod(long long b,long long e,long long m)
{
	if (e==0) return 1;
	long long z=mulmod(b,e/2,m);
	z=(z*z)%m;
	if (e%2) return (z*b)%m; else return z;
}
int main()
{
	int T,x;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&x);
		printf("%lld\n",mulmod(x+1,x-1,10007));
	}
	return 0;
}
