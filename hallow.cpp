// 2008-06-21
// updated 2023-10-20 to fix compilation errors after rejudging
#include <stdio.h>
#include <string.h>
inline void output(int x)
{
	int p10=1;
	while (p10<=x)
		p10*=10;
	p10/=10;
	while (p10)
	{
		putchar_unlocked(x/p10%10+'0');
		p10/=10;
	}
}
inline int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=getchar_unlocked();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
int main()
{
	int i,j,c,n;
	int psum[100001];
	for(;;)
	{
		c=input(); if (c==0) return 0;
		n=input();
		memset(psum,-1,sizeof(int)*c);
		psum[0]=0;
		int ps=0;
		for (i=1; i<=c; i++)
		{
			ps=(ps+input())%c;
			if (psum[ps]!=-1)
				break;
			psum[ps]=i;
		}
		for (j=i+1; j<=n; j++)
			do
				c=getchar_unlocked();
			while (c!=' '&&c!='\n');
		for (j=psum[ps]+1; j<i; j++)
			output(j),putchar_unlocked(' ');
		output(i),putchar_unlocked('\n');
	}
}
