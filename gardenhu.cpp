// 2009-03-01
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
typedef pair<int,int> PII;
typedef long long LL;
int cross(PII O,PII A,PII B)
{
	return (A.first-O.first)*(B.second-O.second)-(A.second-O.second)*(B.first-O.first);
}
double dist(PII A,PII B)
{
	return sqrt((A.first-B.first)*(A.first-B.first)+(A.second-B.second)*(A.second-B.second));
}
int main()
{
	scanf("%d",&T);
	while (T--)
	{
		int n,k,t,i,m;
		PII P[11000],H[22000];
		scanf("%d",&n);
		for (i=0; i<n; i++)
			scanf("%d %d",&P[i].first,&P[i].second);
		k=0;
		sort(P,P+n);
		for (i=0; i<n; i++)
		{
			while (k>=2&&cross(H[k-2],H[k-1],P[i])<=0)
				k--;
			H[k++]=P[i];
		}
		for (i=n-2,t=k+1; i>=0; i--)
		{
			while (k>=t&&cross(H[k-2],H[k-1],P[i])<=0)
				k--;
			H[k++]=P[i];
		}
		double ans=0;
		for (i=0; i<k-1; i++)
			ans+=dist(H[i],H[i+1]);
		printf("%.2f\n",ans);
	}
	return 0;
}

