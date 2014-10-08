// 2009-12-24
#include <iostream>
using namespace std;
long long fusc(long long x)
{
	//The non-recursive version.
	long long n_a=1,n_b=0,a=x,b=x-1;
	//compute n_a * fusc(a) + n_b * fusc(b); a=b+1
	for (;;)
		if (b)
			if (a%2) //a odd, b=a-1
			{
				n_b+=n_a;
				b>>=1;
				a=b+1;
			}
			else //a even, b=a-1
			{
				n_a+=n_b;
				a>>=1;
				b=a-1;
			}
		else //b=0, a=1
			return n_a;
}
int main()
{
	int x;
	for (;;)
	{
		scanf("%d",&x);
		if (!x) return 0;
		printf("%lld/%lld\n",fusc(x),fusc(x+1));
	}
}
