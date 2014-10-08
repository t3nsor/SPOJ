// 2008-06-20
#include <cstdio>
#include <algorithm>
#define getchar getchar_unlocked
using namespace std;
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=getchar();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
int main()
{
	int i,N,M,D,L,c,a[50000];
	N=input();
	M=input();
	D=input();
	L=input();
	c=0;
	for (i=0; i<N; i++)
		a[i]=input();
	sort(a,a+N);
	for (i=0; i<N; i++)
		if (a[i]-D*(c/M)>=L)
			c++;
    printf("%d\n",c);
    return 0;
}
