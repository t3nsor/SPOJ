// 2009-03-01
#include <iostream>
using namespace std;
int main()
{
	static int dp[2001]={0};
	int S,N,i,size,val,j;
	scanf("%d %d",&S,&N);
	for (i=0; i<N; i++)
	{
		scanf("%d %d",&size,&val);
		for (j=S; j>=size; j--)
			dp[j]=max(dp[j],dp[j-size]+val);
	}
	printf("%d\n",dp[S]);
	return 0;
}
