// 2009-04-21
#include <cstdio>
#include <cassert>
#include <algorithm>
using namespace std;
double input()
{
	int x=0;
	int y=0;
	int z=1;
	char c;
	bool minus=false;
	int cnt=0;
	do
	{
		c=getchar_unlocked();
	}
	while (c<=32);
	for(;;)
	{
		if (c=='-')
		{
			minus=true;
			c=getchar_unlocked();
			continue;
		}
		if (c<=32)
			return minus?-x:x;
		else if (c=='.')
		{
			c=getchar_unlocked();
			break;
		}
		x=10*x+c-'0';
		c=getchar_unlocked();
	}
	//c=='.'
	for(;;)
	{
		if (c<=32)
			return minus?-x-double(y)/z:x+double(y)/z;
		y=(y<<1)+(y<<3)+c-'0';
		z=(z<<1)+(z<<3);
		c=getchar_unlocked();
	}
}
int main()
{
	int N,i;
	double a,b,c,d;
	scanf("%d",&N);
	double m0=1000000000.0;
	double M0=-1000000000.0;
	double m1=1000000000.0;
	double M1=-1000000000.0;
	double m2=1000000000.0;
	double M2=-1000000000.0;
	double m3=1000000000.0;
	double M3=-1000000000.0;
	double m4=1000000000.0;
	double M4=-1000000000.0;
	double m5=1000000000.0;
	double M5=-1000000000.0;
	double m6=1000000000.0;
	double M6=-1000000000.0;
	double m7=1000000000.0;
	double M7=-1000000000.0;
	for (i=0; i<N; i++)
	{
		a=input();
		b=input();
		c=input();
		d=input();
		if (m0>a+b+c+d)
			m0=a+b+c+d;
		if (M0<a+b+c+d)
			M0=a+b+c+d;

		if (m1>a+b+c-d)
			m1=a+b+c-d;
		if (M1<a+b+c-d)
			M1=a+b+c-d;

		if (m2>a+b-c+d)
			m2=a+b-c+d;
		if (M2<a+b-c+d)
			M2=a+b-c+d;

		if (m3>a+b-c-d)
			m3=a+b-c-d;
		if (M3<a+b-c-d)
			M3=a+b-c-d;

		if (m4>a-b+c+d)
			m4=a-b+c+d;
		if (M4<a-b+c+d)
			M4=a-b+c+d;

		if (m5>a-b+c-d)
			m5=a-b+c-d;
		if (M5<a-b+c-d)
			M5=a-b+c-d;

		if (m6>a-b-c+d)
			m6=a-b-c+d;
		if (M6<a-b-c+d)
			M6=a-b-c+d;

		if (m7>a-b-c-d)
			m7=a-b-c-d;
		if (M7<a-b-c-d)
			M7=a-b-c-d;
	}
	double ans=0.000;
	if (ans<M0-m0)
		ans=M0-m0;
	if (ans<M1-m1)
		ans=M1-m1;
	if (ans<M2-m2)
		ans=M2-m2;
	if (ans<M3-m3)
		ans=M3-m3;
	if (ans<M4-m4)
		ans=M4-m4;
	if (ans<M5-m5)
		ans=M5-m5;
	if (ans<M6-m6)
		ans=M6-m6;
	if (ans<M7-m7)
		ans=M7-m7;
	printf("%.3lf\n",ans);
	return 0;
}
