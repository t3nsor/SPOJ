// 2008-04-20
#include <stdio.h>
#include <algorithm>
using namespace std;
int N,C;
static int a[100000];
bool doable(int dist)
{
	int _count=1;
	int i=0,j;
	while (_count<C)
	{
		j=i+1;
		while (j<N && a[j]-a[i]<dist) j++;
		if (j==N) return false;
		_count++;
		i=j;
	}
	return true;
}
int main()
{
	int i,t,j;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d %d",&N,&C);
		for (j=0; j<N; j++)
			scanf("%d",&a[j]);
		sort(a,a+N);
		int l=1;
		int u=(a[N-1]-a[0])/(C-1);
		int m;
		while (l<u)
		{
			m=(l+u+1)/2;
			if (doable(m))
				l=m;
			else
				u=m-1;
		}
		printf("%d\n",l);
	}
	return 0;
}
