// 2009-08-18
#include <iostream>
using namespace std;
int in() //INPUT HACK FOR EXTREME SPEED
{
	int x=0;
	char c;
	do
	{
		c=getchar_unlocked();
	}
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int BIT[500000];
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
	for (;x<N;x|=x+1)
		BIT[x]=max(BIT[x],y);
}
int main() //MOST ANNOYING INPUT FORMAT EVER
{
	int i;
	int first[500000];
	int second[500000];
	int third[500000];
	int res=0;
	N=in();
	for (i=0; i<N; i++)
		first[i]=in()-1;
	for (i=0; i<N; i++)
		second[in()-1]=i;
	for (i=0; i<N; i++)
		third[in()-1]=500001-i;
	for (i=0; i<N; i++)
	{
		//printf("%d %d %d\n",i,second[first[i]],third[first[i]]);
		if (query(second[first[i]])<third[first[i]]) //ok
		{
			res++;
			insert(second[first[i]],third[first[i]]);
		}
	}
	printf("%d\n",res);
	return 0;
}
