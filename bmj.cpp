// 2008-06-26
#include <cstdio>
#include <cmath>
int input()
{
	char c=getchar();
	if (c==-1) return -1;
	else ungetc(c,stdin);
	int x=0;
	for(;;)
	{
		c=getchar();
		if (c==' '||c=='\n') return x;
		x=10*x+c-'0';
	}
}
int main()
{
	for(;;)
	{
		int x=input();
		int X,Y;
		if (x==-1) return 0;
		int l=1e-7+(3.0+sqrt(12.0*x-15.0))/6.0;
		if (x<=3*l*l-2*l+1)
		{
			X=3*l*l-2*l+1-x;
			Y=l-X;
		}
		else if (x<=3*l*l-l+1)
		{
			X=3*l*l-2*l+1-x;
			Y=l;
		}
		else if (x<=3*l*l+1)
		{
			X=-l;
			Y=3*l*l+1-x;
		}
		else if (x<=3*l*l+l+1)
		{
			X=x-(3*l*l+l+1);
			Y=3*l*l+1-x;
		}
		else if (x<=3*l*l+2*l+1)
		{
			X=x-(3*l*l+l+1);
			Y=-l;
		}
		else //x<=3*l*l+3*l+1
		{
			X=l;
			Y=x-(3*l*l+3*l+1);
		}
		printf("%d %d\n",X,Y);
	}
}
