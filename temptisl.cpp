// 2009-04-10
#include <iostream>
#include <cstring>
using namespace std;
int main()
{
	long long N,K,A,B,i,j;
	long long ways[55][55];
	for(;;)
	{
		scanf("%lld %lld",&N,&K);
		if (!~N) return 0;
		scanf("%lld %lld",&A,&B); A--,B--;
		memset(ways,0,sizeof(ways));
		ways[0][A]=1;
		for (i=1; i<=K; i++)
			for (j=0; j<N; j++)
				ways[i][j]=ways[i-1][(j+N-1)%N]+ways[i-1][(j+1)%N];
		printf("%lld\n",ways[K][B]);
	}
}
