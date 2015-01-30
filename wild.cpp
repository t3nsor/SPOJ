// 2008-08-30
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;
typedef pair<int,int> PII;
typedef pair<int,PII> PIPII;
map<int,int> S;
typedef long long LL;
LL area;
void ins(PII p)
{
	map<int,int>::iterator It=S.lower_bound(p.first),last=It,It2;
	//S.begin() < It < S.end() always
	//both It-1 and It are valid iterators
	if (p.second<=It->second)
		return;
	It--;
	area+=(LL)(p.first-It->first)*(LL)(p.second-last->second);
	if (p.first==last->first&&p.second>last->second)
		last++;
	It2=It;
	while (p.second>=It->second)
	{
		It2--;
		area+=(LL)(It->first-It2->first)*(LL)(p.second-It->second);
		It=It2;
	}
	It++;
	S.erase(It,last);
	S.insert(p);
}
PIPII a[100010];
int in()
{
	char c;
	int res=0;
	for(;;)
	{
#ifdef ONLINE_JUDGE
		c=getchar_unlocked();
#else
		c=getchar();
#endif
		if (c<=32)
			return res;
		res=(res<<1)+(res<<3)+c-'0';
	}
}
int main()
{
	int n,m,i;
	for(;;)
	{
		n=in();
		m=in();
		if (!m) return 0;
		LL vol=m; vol*=vol*vol;
		for (i=0; i<n; i++)
		{
			a[i].first=in();
			a[i].second.first=in();
			a[i].second.second=in();
		}
		sort(a,a+n);
		S.clear();
		S.insert(a[n-1].second);
		S.insert(PII(0,1000000));
		S.insert(PII(1000000,0));
		area=(LL)a[n-1].second.first*(LL)a[n-1].second.second;
		for (i=n-2; i>=0; i--)
		{
			vol-=area*(LL)(a[i+1].first-a[i].first);
			ins(a[i].second);
		}
		vol-=area*(LL)a[0].first;
		printf("%lld\n",vol);
	}
}
