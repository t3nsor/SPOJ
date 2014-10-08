// 2008-07-26
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int n;
int x[91];
int y[91];
int h[91];
int memo[91];
int height(int k)
{
	int i;
	if (memo[k]!=-1)
		return memo[k];
	int top=0;
	for (i=0; i<n; i++)
		if (x[i]<x[k]&&y[i]<y[k]||x[i]<y[k]&&y[i]<x[k])
			top=max(top,height(i));
	return memo[k]=h[k]+top;
}
int main()
{
	int i;
	for(;;)
	{
		scanf("%d",&n);
		if (!n) return 0;
		for (i=0; i<n; i++)
		{
			scanf("%d %d %d",x+i,y+i,h+i);
			x[i+n]=y[i]; y[i+n]=h[i]; h[i+n]=x[i];
			x[i+2*n]=h[i]; y[i+2*n]=x[i]; h[i+2*n]=y[i];
		}
		x[3*n]=2000000000; y[3*n]=2000000000; h[3*n]=0;
		n=3*n+1;
		memset(memo,-1,sizeof(memo));
		printf("%d\n",height(n-1));
	}
};
