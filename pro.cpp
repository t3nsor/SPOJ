// 2009-05-05
#include <iostream>
#include <set>
using namespace std;
int main()
{
	int N,n,x;
	scanf("%d",&N);
	long long res=0;
	multiset<int> S;
	while (N--)
	{
		scanf("%d",&n);
		while (n--)
		{
			scanf("%d",&x);
			S.insert(x);
		}
		multiset<int>::iterator begin=S.begin();
		multiset<int>::iterator end=S.end(); end--;
		res+=*end-*begin;
		S.erase(begin); S.erase(end);
	}
	printf("%lld\n",res);
	return 0;
}
