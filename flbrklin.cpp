// 2009-05-01
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;
int main()
{
	int T,N,i,x,y;
	pair<int,int> A[30000];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		for (i=0; i<N; i++)
		{
			scanf("%d %d",&x,&y);
			A[i]=make_pair(x-y,x+y);
		}
		sort(A,A+N);
		set<int> S;
		for (i=0; i<N; i++)
		{
			set<int>::iterator It=S.upper_bound(A[i].second);
			if (It!=S.begin())
			{
				It--;
				S.erase(It);
			}
			S.insert(A[i].second);
		}
		printf("%d\n",S.size());
	}
	return 0;
}
