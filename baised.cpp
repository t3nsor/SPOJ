// 2009-06-28
// modified 2010-10-07
#include <cstdio>
#include <algorithm>
using namespace std;
#define i64 long long
#define GC getchar_unlocked
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
	char c;
	i64 answer;
	int T,i,N,j;
	static int a[1000000];
	T=input();
	for (i=0; i<T; i++)
	{
		GC();
		N=input();
		for (j=0; j<N; j++)
		{
			do c=GC(); while (c!=' ');
			a[j]=input()-1;
		}
		sort(a,a+N);
		answer=0;
		for (j=0; j<N; j++)
			if (a[j]<=j)
				answer+=(i64)j-(i64)a[j];
			else
				answer+=(i64)a[j]-(i64)j;
		printf("%lld\n",answer);
	}
	return 0;
}
