// 2008-10-24
#include <iostream>
#include <algorithm>
using namespace std;
int id[100000];
int sz[100000];
pair<int,pair<int,int> > a[100000];
int main()
{
	int V,E,i,v1,v2,wt;
	scanf("%d %d",&V,&E);
	for (i=0; i<V; i++)
	{
		id[i]=i;
		sz[i]=1;
	}
	for (i=0; i<E; i++)
	{
		scanf("%d %d %d",&v1,&v2,&wt);
		a[i]=make_pair(wt,make_pair(v1-1,v2-1));
	}
	sort(a,a+E);
	int edges=0;
	long long res=0;
	i=0;
	while (edges<V-1)
	{
		int A=a[i].second.first;
		int B=a[i].second.second;
		while (A!=id[A])
			A=id[A];
		while (B!=id[B])
			B=id[B];
		if (A==B)
		{
			i++;
			continue;
		}
		res+=a[i].first;
		if (sz[A]<sz[B])
		{
			id[A]=B;
			sz[B]+=sz[A];
		}
		else
		{
			id[B]=A;
			sz[A]+=sz[B];
		}
		edges++;
		i++;
	}
	printf("%lld\n",res);
	return 0;
}
