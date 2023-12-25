// 2008-08-14
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int N,x,i,j;
	char s[100];
	scanf("%d",&N);
	for (j=1; j<=N; j++)
	{
		printf("%d ",j);
		scanf("%d %s",&x,s);
		x--;
		for (i=0; i<x; i++)
			putchar(s[i]);
		for (i=x+1; s[i]; i++)
			putchar(s[i]);
		putchar('\n');
	}
	return 0;
}
