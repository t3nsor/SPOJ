// 2008-04-19
#include <iostream>
using namespace std;
int main()
{
	static bool cube[1000000];
	static int cbrt[1000000];
	int i,a,b,c;
	for (i=1; i<=100; i++)
	{
		cube[i*i*i]=true;
		cbrt[i*i*i]=i;
	}
	for (a=2; a<=100; a++)
	{
		if (a==9) continue; //stupid
		int a3=a*a*a;
		for (b=1; b<a; b++)
		{
			int b3=b*b*b;
			for (c=b; c<a; c++)
			{
				int d3=a3-b3-c*c*c;
				if (d3<c*c*c)
					continue;
				if (d3<=0)
					continue;
				if (!cube[d3])
					continue;
				int d=cbrt[d3];
				printf("Cube = %d, Triple = (%d,%d,%d)\n",a,b,c,d);
			}
		}
	}
	return 0;
}
