// 2008-12-07
#include <iostream>
#include <algorithm>
#define next(x) ((x)==k-1?0:(x)+1)
using namespace std;
typedef pair<int,int> PII;
typedef long long LL;
LL cross(PII O,PII A,PII B)
{
	return LL(A.first-O.first)*(B.second-O.second)-LL(A.second-O.second)*(B.first-O.first);
}
LL sqrdist(PII p1,PII p2)
{
	return LL(p1.first-p2.first)*(p1.first-p2.first)+LL(p1.second-p2.second)*(p1.second-p2.second);
}
void fix()
{
	char c;
	do
		c=getchar_unlocked();
	while (c<=32);
	ungetc(c,stdin);
}
int in()
{
	fix();
	char c;
	bool neg=false;
	int x=0;
	for(;;)
	{
		c=getchar_unlocked();
		if (c<=32)
			return neg?-x:x;
		if (c=='-')
			neg=true;
		else
			x=(x<<1)+(x<<3)+c-'0';
	}
}
int main()
{
	int n,k,t,i,T,j;
	LL m;
	PII P[100000],H[200000];
	T=in();
	while (T--)
	{
//		getchar_unlocked();
		n=in();
		for (i=0; i<n; i++)
		{
			P[i].first=in();
			P[i].second=in();
		}
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
		k--;
		j=0;
		m=0;
		for (i=0; i<k; i++)
		{
			while (sqrdist(H[i],H[next(j)])>sqrdist(H[i],H[j]))
				j=next(j);
			m=max(m,sqrdist(H[i],H[j]));
		}
		printf("%lld\n",m);
	}
	return 0;
}

