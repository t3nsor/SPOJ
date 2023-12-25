// 2009-04-13
#include <iostream>
typedef long long LL;
using namespace std;
int n1,n2;
static int Left1[100001];
static int Right1[100001];
static int Left2[100001];
static int Right2[100001];
LL hash1(int root)
{
	LL h1,h2;
	if (root<0)
	{
		h1=root;
		h2=1337;
	}
	else
	{
		h1=hash1(Left1[root]);
		h2=hash1(Right1[root]);
		if (h1<h2) swap(h1,h2);
	}
	return (h1+729927)^(h2*5291);
}
LL hash2(int root)
{
	LL h1,h2;
	if (root<0)
	{
		h1=root;
		h2=1337;
	}
	else
	{
		h1=hash2(Left2[root]);
		h2=hash2(Right2[root]);
		if (h1<h2) swap(h1,h2);
	}
	return (h1+729927)^(h2*5291);
}
int get()
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
int main()
{
	int i,j;
	n1=get();
	for (i=1; i<=n1; i++)
		Left1[i]=get(),Right1[i]=get();
	n2=get();
	if (n1-n2)
	{
		puts("Fred and Mary have different mobiles.");
		return 0;
	}
	for (i=1; i<=n2; i++)
		Left2[i]=get(),Right2[i]=get();
	LL h1=hash1(1);
	LL h2=hash2(1);
	puts(h1-h2?"Fred and Mary have different mobiles.":"Fred and Mary might have the same mobile.");
	return 0;
}
