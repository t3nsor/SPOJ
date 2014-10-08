// 2010-11-15
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
#include <map>
using namespace std;
int main()
{
	int t;
	scanf("%d",&t);
	while (t--)
	{
		int n,i;
		int area[40000];
		pair<int,int> a[80000];
		scanf("%d",&n);
		for (i=0; i<n; i++)
		{
			area[i]=0;
			int b,e;
			scanf("%d %d",&b,&e);
			a[2*i]=make_pair(b,i);
			a[2*i+1]=make_pair(e+1,1e6+i);
		}
		sort(a,a+2*n);
		set<int> active;
		for (i=0; i<2*n; i++)
		{
			if (!active.empty())
			{
				set<int>::iterator It=active.end();
				--It;
				area[*It]+=a[i].first-a[i-1].first;
			}
			if (a[i].second<1e6) //start
				active.insert(a[i].second);
			else
				active.erase(a[i].second-1e6);
		}
		int cnt=0;
		for (i=0; i<n; i++)
			if (area[i]) cnt++;
		printf("%d\n",cnt);
	}
	return 0;
}
