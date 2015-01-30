// 2009-08-18
// corrected 2014-01-29
// The simple solution to NICEDAY. Doesn't generalize to LIS2 though;
// see niceday-1.cpp for another idea
#include <iostream>
using namespace std;
int in() //INPUT HACK FOR EXTREME SPEED
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
static int BITPool[1000010]={0}; //DON'T RE-INITIALIZE MEMORY
int* BIT;
int N;
int query(int x)
{
	int res=0;
	for (;~x;x=(x&(x+1))-1)
		res=max(res,BIT[x]);
	return res;
}
void insert(int x,int y)
{
	for (;x<=N;x|=x+1)
		BIT[x]=max(BIT[x],y);
}
int main()
{
	int T=in();
	int i;
	BIT=BITPool;
	int first[100000];
	int second[100000];
	while (T--)
	{
		int res=0;
		N=in();
		for (i=0; i<N; i++) //BUCKET SORT
		{
			int x=in();
			first[x-1]=in();
			second[x-1]=100001-in();
		}
		for (i=0; i<N; i++)
			if (query(first[i])<second[i]) //ok
			{
				res++;
				insert(first[i],second[i]);
			}
		printf("%d\n",res);
		BIT+=100001;
	}
	return 0;
}
