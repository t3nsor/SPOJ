// 2008-07-28
#define _CRT_SECURE_NO_WARNINGS
#ifdef ONLINE_JUDGE
#define GC getchar_unlocked
#else
#define GC getchar
#endif
#include <iostream>
using namespace std;
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n'||c==-1)
			return x;
		x=10*x+c-'0';
	}
}
int main()
{
	static int I[100002];
	static int h[100002];
	static int l[100002];
	static int r[100002];
	int n,i,top;
	for(;;)
	{
		n=input();
		if (!n) return 0;
		h[0]=-1;
		h[n+1]=-1;
		top=0;
		for (i=1; i<=n; i++)
			h[i]=input();
		for (i=1; i<=n+1; i++)
		{
			while (top&&h[I[top-1]]>h[i])
				r[I[--top]]=i;
			I[top++]=i;
		}
		top=0;
		for (i=n; i>=0; i--)
		{
			while (top&&h[I[top-1]]>h[i])
				l[I[--top]]=i;
			I[top++]=i;
		}
		long long best=0;
		for (i=1; i<=n; i++)
			best=max(best,(long long)h[i]*(r[i]-l[i]-1));
		printf("%lld\n",best);
	}
}
