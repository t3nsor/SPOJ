// 2009-03-01
#include <iostream>
using namespace std;
int in()
{
	int x=0;
	char c;
	do
	{
		c=getchar_unlocked();
	}
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int main()
{
	int T,R,C,i,j,x;
	T=in();
	while (T--)
	{
		int dp[102][102]={0};
		R=in(); C=in();
		for (i=1; i<=R; i++)
			for (j=1; j<=C; j++)
			{
				x=in();
				dp[i][j]=x+max(max(dp[i-1][j-1],dp[i-1][j]),dp[i-1][j+1]);
			}
		x=0;
		for (i=1; i<=C; i++)
			x=max(x,dp[R][i]);
		printf("%d\n",x);
	}
	return 0;
}
