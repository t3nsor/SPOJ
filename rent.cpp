// 2009-05-05
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	int T,N,i,begin,length,price;
	int b[11111];
	int e[11111];
	int p[11111];
	scanf("%d",&T);
	while (T--)
	{
		int dp[22222];
		pair<int,int> a[22222];
		scanf("%d",&N);
		for (i=0; i<N; i++)
		{
			scanf("%d %d %d",&begin,&length,&price);
			b[i]=begin;
			e[i]=begin+length;
			p[i]=price;
			a[2*i]=make_pair(begin,2*i);
			a[2*i+1]=make_pair(begin+length,2*i+1);
		}
		sort(a,a+2*N);
		for (i=0; i<2*N; i++)
			if (a[i].second%2) //end
				e[a[i].second/2]=i;
			else
				b[a[i].second/2]=i;
		for (i=0; i<N; i++)
		{
			a[2*i]=make_pair(b[i],2*i+1);
			a[2*i+1]=make_pair(e[i],2*i);
		}
		sort(a,a+2*N);
		dp[0]=0;
		for (i=1; i<2*N; i++)
			if (a[i].second%2) //begin
				dp[i]=dp[i-1];
			else //end
				dp[i]=max(dp[i-1],p[a[i].second/2]+dp[b[a[i].second/2]-1]);
		printf("%d\n",dp[2*N-1]);
	}
	return 0;
}
