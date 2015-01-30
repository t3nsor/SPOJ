// 2008-01-29
#include <stdio.h>
#include <string.h>
int main()
{
	int y,x,l,i,j;
	char s[201];
	char t;
	for(;;)
	{
		scanf("%d",&x);
		if (x==0) break;
		scanf("%s",&s);
		l=strlen(s);
		y=l/x;
		for (j=x; j<l; j+=2*x)
			for (i=0; i<x/2; i++)
			{
				t=s[j+i];
				s[j+i]=s[j+x-i-1];
				s[j+x-i-1]=t;
			}
		for (i=0; i<x; i++)
			for (j=i; j<l; j+=x)
				putchar(s[j]);
		putchar('\n');
	}
	return 0;
}
