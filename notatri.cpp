// 2008-08-04
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
using namespace std;
int main()
{
	int a[2001];
	int i,j,N;
	for(;;)
	{
		scanf("%d",&N);
		if (!N) return 0;
		for (i=0; i<N; i++)
			scanf("%d",a+i);
		a[N++]=-1;
		sort(a,a+N);
		int l,u,m;
		long long total=0;
		for (i=3; i<N; i++)
			for (j=2; j<i; j++)
			{
				l=0;
				u=j-1;
				while (u>l)
				{
					m=(l+u+1)/2;
					if (a[m]+a[j]>=a[i])
						u=m-1;
					else
						l=m;
				}
				total+=l;
			}
		printf("%lld\n",total);
	}
}
