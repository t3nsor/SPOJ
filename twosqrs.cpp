// 2008-07-21
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
int main()
{
	int c,i,j,t;
	long long n;
	static long long sqr[1000001];
	for (i=0; i<=1000000; i++)
		sqr[i]=(long long)i*i;
	scanf("%d",&c);
	for (t=0; t<c; t++)
	{
		scanf("%lld",&n);
		i=0;
		j=sqrt((long double)n);
		while (i<=j)
		{
			if (sqr[i]+sqr[j]==n)
				break;
			else if (sqr[i]+sqr[j]<n)
				i++;
			else
				j--;
		}
		if (sqr[i]+sqr[j]==n)
			printf("Yes\n");
		else
			printf("No\n");
	}
	return 0;
}
