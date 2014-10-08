// 2009-04-22
// modified 2014-10-07
#include <iostream>
typedef long long __int64;
using namespace std;
int main()
{
	int deg,i,j,k,x,cnt=0;
	int a[1000];
	for (;;)
	{
		scanf("%d",&deg);
		if (!~deg) return 0;
		printf("Case %d:\n",++cnt);
		for (i=deg; i>=0; i--)
			scanf("%d",a+i);
		scanf("%d",&k);
		for (i=0; i<k; i++)
		{
			scanf("%d",&x);
			__int64 result=0;
			for (j=deg; j>=0; j--)
				result=result*x+a[j];
			printf("%lld\n",result);
		}
	}
}
