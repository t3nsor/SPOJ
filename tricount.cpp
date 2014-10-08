// 2008-04-20
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
		int64 uptri=N*(N+1)*(N+2)/6;
		int64 downtri=(2*N*N*N+3*N*N-2*N-(N%2?3:0))/24;
		printf("%lld\n",uptri+downtri);
	}
	return 0;
}
