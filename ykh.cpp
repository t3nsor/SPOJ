// 2023-11-03
// The idea is pretty simple. I coded this up back in 2010, but kept getting WA
// due to overflow. Finally I came back and fixed it :)
//
// If there's a predetermined set of certificates, then for each client, the
// optimal decision is to show them whichever certificate shows a price closest
// to a[i]/2b[i], assuming that the set of certificates is nonempty and that at
// least one certificate results in a positive profit. From this it follows that
// the list of clients is sorted in order of their a/b values, then each
// certificate should be used for a (possibly empty) contiguous subarray of the
// list of clients. This immediately suggests the O(C^2) DP approach below.
#include <iostream>
#include <algorithm>
using namespace std;
bool comp(const pair<int,int>& x,const pair<int,int>& y)
{
	return x.first*y.second<y.first*x.second;
}
int main()
{
	int T;
	int B,N;
	int x,y;
	int i,j;
	long double asum[2001],bsum[2001];
	long double dp[2001];
	pair<int,int> A[2000];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d %d",&B,&N);
		for (i=0; i<N; i++)
			scanf("%d %d",&A[i].first,&A[i].second);
		sort(A,A+N,comp);
		asum[0]=0;
		bsum[0]=0;
		for (i=0; i<N; i++)
		{
			asum[i+1]=asum[i]+A[i].first;
			bsum[i+1]=bsum[i]+A[i].second;
		}
		dp[0]=0.0;
		for (i=1; i<=N; i++) //best profit for first i clients
		{
			dp[i]=dp[i-1];
			for (j=0; j<i; j++)
				dp[i]=max(dp[i],-B+dp[j]+(asum[i]-asum[j])*
				  (asum[i]-asum[j])/(4.0*(bsum[i]-bsum[j])));
		}
		printf("%Lf\n",dp[N]);
	}
	return 0;
}
