// 2008-06-21
#include <cstdio>
int main()
{
	int x,y,z;
	for(;;)
	{
		x=getchar()-'0';
		y=getchar()-'0';
		getchar();
		z=getchar()-'0';
		if (x==0&&y==0&&z==0)
			return 0;
		int X=10*x+y;
		while (z--)
			X*=10;
		int p2=1;
		while (p2<=X)
			p2<<=1;
		p2>>=1;
		printf("%d\n",2*(X-p2)+1);
		getchar();
	}
}
