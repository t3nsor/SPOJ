// 2009-12-24
#include <iostream>
#include <map>
using namespace std;
map<long long,long long> memo;
long long fusc(long long x)
{
	if (x<2) return x;
	int z;
	if (z=memo[x])
		return z;
	else
		if (x%2) //odd
			return memo[x]=fusc(x/2)+fusc(1+x/2);
		else
			return memo[x]=fusc(x/2);
}
int main()
{
	int x;
	for (;;)
	{
		memo.clear();
		scanf("%d",&x);
		if (!x) return 0;
		printf("%lld/%lld\n",fusc(x),fusc(x+1));
	}
}
