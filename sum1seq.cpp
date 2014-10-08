// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int t,s,n,i;
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d %d",&n,&s);
		if (2*abs(s)>n*(n-1)||n%4<2&&s%2||n%4>1&&s%2==0)
		{
			printf("No\n\n");
			continue;
		}
		printf("0\n");
		int num=0;
		for (i=n-1; i; i--)
		{
			num+=s>0?(s-=i,1):(s+=i,-1);
			printf("%d\n",num);
		}
		putchar('\n');
	}
	return 0;
}
