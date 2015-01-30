// 2008-08-30
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#define i64 long long
using namespace std;
#define MAX(a,b) ((a)>(b)?(a):(b))
int BIT[50000];
int query(int x)
{
	int X;
	for (X=x-1; X>=0; X=(X&(X+1))-1)
		x-=BIT[X];
	return x;
}
int N;
void del(int x)
{
	for (;x<N;x|=(x+1))
		BIT[x]++;
}
int in()
{
	char c;
	int res=0;
	for(;;)
	{
#ifdef ONLINE_JUDGE
		c=getchar_unlocked();
#else
		c=getchar();
#endif
		if (c<=32) return res;
		res=(res<<1)+(res<<3)+c-'0';
	}
}
int main()
{
	int i,j,k,last,num;
	i64 t;
	i64 time[50000];
	pair<int,int> a[50000];
	int A[50000];
	N=in();
	for (i=0; i<N; i++)
	{
		a[i]=make_pair(in(),i);
		A[i]=i;
	}
	sort(a,a+N);
	i=0;
	last=0;
	num=N;
	t=0;
	while (i<N)
	{
		j=i;
		while (j<N&&a[j].first==a[i].first)
			j++;
		t+=(i64)num*(i64)(a[i].first-last);
		for (k=i; k<j; k++)
			time[a[k].second]=t-(i64)num+(i64)query(a[k].second)+1;
		for (k=i; k<j; k++)
			del(a[k].second);
		num-=(j-i);
		last=a[i].first;
		i=j;
	}
	for (i=0; i<N; i++)
		printf("%lld\n",time[i]);
	return 0;
}
