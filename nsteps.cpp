// 2008-01-30
#include <stdio.h>
int main()
{
	int N,i,x,y;
	scanf("%d",&N);
	for (i=0; i<N; i++)
	{
		scanf("%d %d",&x,&y);
		if (x==y)
			printf("%d\n",x+2*(x/2));
		else if (x-y==2)
			printf("%d\n",x+2*((x-2)/2));
		else
			printf("No Number\n");
	}
	return 0;
}
