// 2008-07-23
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
#include <set>
#include <stack>
using namespace std;
struct Triangle
{
	long double l1;
	long double l2;
	Triangle(){}
	Triangle(long double a,long double b,long double c)
	{
		if (a>b)
			swap(a,b);
		if (b>c)
			swap(b,c);
		if (a>b)
			swap(a,b);
		l1=b/a;
		l2=c/a;
	}
};
bool equ(long double x,long double y)
{
	return x>=y&&x-y<1e-8||y>=x&&y-x<1e-8;
}
bool below(long double x,long double y)
{
	return !equ(x,y)&&x<y;
}
bool operator<(Triangle t1,Triangle t2)
{
	if (below(t1.l1,t2.l1))
		return true;
	else if (equ(t1.l1,t2.l1))
		if (below(t1.l2,t2.l2))
			return true;
		else
			return false;
	else
		return false;
}
int main()
{
	int N;
	scanf("%d",&N);
	long double a,b,c;
	while (N--)
	{
		set<Triangle> S;
		stack<Triangle> s;
		scanf("%Lf %Lf %Lf",&a,&b,&c);
		s.push(Triangle(a,b,c));
		while (!s.empty())
		{
			Triangle T=s.top();
			s.pop();
			if (S.find(T)!=S.end())
				continue;
			else
				S.insert(T);
			long double cos1=(1.0L+T.l2*T.l2-T.l1*T.l1)/(2.0L*T.l2);
			long double cos2=(T.l1*T.l1+T.l2*T.l2-1.0L)/(2.0L*T.l1*T.l2);
			s.push(Triangle(1.0L,T.l2/2.0L,sqrt(1.0L+T.l2*T.l2/4.0L-T.l2*cos1)));
			s.push(Triangle(T.l1,T.l2/2.0L,sqrt(T.l1*T.l1+T.l2*T.l2/4.0L-T.l1*T.l2*cos2)));
		}
		printf("%d\n",S.size());
	}
	return 0;
}
