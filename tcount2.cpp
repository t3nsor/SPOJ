// 2008-06-23
// modified 2014-10-07
#include <stdio.h>
typedef long long int64;
int main()
{
	int i,T;
	int64 N;
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		N=0;
		scanf("%lld",&N);
		printf("%lld\n",3*N*N*N+(2*N*N*N+9*N*N+2*N-(N%2?1:0))/4);
	}
	return 0;
}
