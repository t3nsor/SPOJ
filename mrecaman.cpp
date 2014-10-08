// 2009-03-07
#include <iostream>
using namespace std;
int main()
{
	int a[500001];
	static int vis[4000000]={0};
	int i,x;
	a[0]=0;
	vis[0]=1;
	int M=0;
	for (i=1; i<=500000; i++)
	{
		x=a[i-1]-i;
		if (x<=0||vis[x])
			a[i]=a[i-1]+i;
		else
			a[i]=x;
		vis[a[i]]=1;
	}
	for(;;)
	{
		scanf("%d",&i);
		if (!~i) return 0;
		printf("%d\n",a[i]);
	}
}
