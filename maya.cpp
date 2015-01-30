// 2008-01-30
#include <stdio.h>
const int placevalue[7]={1,20,360,7200,144000,2880000,57600000};
int main()
{
	int n,N,i,j,k,d;
	char c;
	for (;;)
	{
		scanf("%d\n",&n);
		if (n==0) return 0;
		N=0;
		for (i=n-1; i>=0; i--)
		{
			j=0;
			k=0;
			while ((c=getchar())!='\n')
				if (c=='.')
					j++;
				else if (c=='-')
					k++;
			d=j+5*k;
			N+=d*placevalue[i];
		}
		printf("%d\n",N);
	}
}
