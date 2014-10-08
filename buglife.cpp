// 2008-11-24
#include <iostream>
#include <cstring>
using namespace std;
int in()
{
	int x=0;
	char c;
	for(;;)
	{
		c=getchar_unlocked();
		if (c<=32) return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
int dig[20];
void out(int x)
{
	int cnt=0;
	while (x)
	{
		dig[cnt++]=x%10;
		x/=10;
	}
	while (cnt--)
		putchar_unlocked(dig[cnt]+'0');
}
void eatline()
{
	char c=0;
	while (c!='\n')
		c=getchar_unlocked();
}
void outs(char* s)
{
	while (*s)
		putchar_unlocked(*s++);
}
int main()
{
	//weighted quick-union
	int T,i,N,M,a,b,A,B;
	int id[2000];
	int bit[2000];
	int sz[2000];
	char s[30];
	T=in();
	for (i=1; i<=T; i++)
	{
		outs("Scenario #");
		out(i);
		putchar_unlocked(':');
		putchar_unlocked('\n');
		N=in(),M=in(); //# bugs, # interactions
		while (N--)
		{
			id[N]=N;
			bit[N]=0;
//			sz[N]=1;
		}
		while (M--)
		{
			a=in()-1,b=in()-1;
			A=0,B=0;
			while (a!=id[a])
			{
				bit[a]^=bit[id[a]];
				A^=bit[a];
				a=id[a]=id[id[a]];
			}
			while (b!=id[b])
			{
				bit[b]^=bit[id[b]];
				B^=bit[b];
				b=id[b]=id[id[b]];
			}
			if (a==b&&A==B) //suspicious bugs found
			{
				outs("Suspicious bugs found!\n");
				break;
			}
//			if (sz[a]<sz[b])
//			{
				id[a]=b;
				bit[a]=A^B^1;
/*			}
			else
			{
				id[b]=a;
				bit[b]=A^B^1;
			}*/
		}		
		if (M==-1)
			outs("No suspicious bugs found!\n");
		else while (M--) eatline();
	}
	return 0;
}
