// 2008-01-29
#include <stdio.h>
#include <math.h>
int main()
{
	int t,i,N,a,b,c,x,y;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d",&N);
		a=(int)ceil((sqrt(8*N+1)-1)/2);
		b=(a*(a+1))/2;
		c=b-N+1;
		if (a%2==1)
		{
			x=c;
			y=a-c+1;
		}
		else
		{
			x=a-c+1;
			y=c;
		}
		printf("TERM %d IS %d/%d\n",N,x,y);
	}
	return 0;
}
