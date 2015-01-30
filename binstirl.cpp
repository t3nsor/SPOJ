// 2008-06-26
#include <cstdio>
int main()
{
	int i,d,n,k;
	scanf("%d",&d);
	for (i=0; i<d; i++)
	{
		scanf("%d %d",&n,&k);
		if ((n-k)&((k-1)/2))
			printf("0\n");
		else
			printf("1\n");
	}
	return 0;
}
