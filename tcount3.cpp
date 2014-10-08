// 2008-06-23
#include<cstdio>
int main()
{
	int T,i;
	unsigned long long N;
	scanf("%d",&T);
	for(i=0;i<T;i++)
	{
		scanf("%llu",&N);
		printf("%llu\n",N*(10*N*N+9*N+1));
	}
}
