// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int N,M;
int pages[500];
bool ok(long long x)
{
	int next=0;
	int i;
	for (i=0; i<M; i++)
	{
		long long sum=0;
		do
		{
			sum+=pages[next];
			if (sum<=x)
				next++;
		}
		while (next<N&&sum<=x);
		if (next==N&&sum<=x)
			return true;
	}
	return false;
}
int main()
{
#ifndef ONLINE_JUDGE
	freopen("books.in","r",stdin);
	freopen("books.out","w",stdout);
#endif
	int t,i,j;
	scanf("%d",&t);
	int books[500];
	int work[500][500];
	while (t--)
	{
		scanf("%d %d",&N,&M);
		memset(books,0,sizeof(books));
		long long total=0;
		int _m=0;
		for (i=0; i<N; i++)
		{
			scanf("%d",pages+i);
			total+=pages[i];
			_m=max(_m,pages[i]);
		}
		long long l=max((total+M-1)/M,(long long)_m);
		long long u=total;
		while (u>l)
		{
			long long m=(l+u)/2;
			if (ok(m))
				u=m;
			else
				l=m+1;
		}
		int next=N-1;
		for (i=M-1; i>=0; i--)
		{
			long long sum=0;
			for(;;)
			{
				sum+=pages[next];
				if (sum<=l&&next>=i)
					work[i][books[i]++]=pages[next--];
				else
					break;
			}
		}
		for (i=0; i<M-1; i++)
		{
			for (j=books[i]-1; j>=0; j--)
				printf("%d ",work[i][j]);
			printf("/ ");
		}
		for (i=books[M-1]-1; i; i--)
			printf("%d ",work[M-1][i]);
		printf("%d\n",work[M-1][0]);
	}
}
