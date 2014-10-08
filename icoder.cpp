// 2008-06-20
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
int main()
{
	int N,X,i,p;
	char c;
	for(;;)
	{
		N=input();
		p=16;
		if (N==0)
			return 0;
		for (i=0; i<N; i++)
		{
			c=GC();GC();GC();GC();
			X=input();
			if (c=='A')
				continue;
			int C=0;
			while (C<=16&&(X&1)==0)
			{
				X>>=1;
				C++;
			}
			if (C>=p)
				p=0;
			else
				p-=C;
		}
		printf("%d\n",1<<p);
	}
}
