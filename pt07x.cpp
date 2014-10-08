// 2009-04-05
#include <iostream>
using namespace std;
static int neighbours[222222]={0};
static int Link[222222]={0};
static int first[222222]={0};
static int last[222222]={0};
int top=1;
void f(int parent,int node,int& i,int& n)
{
	i=1;
	n=0;
	for (int I=first[node]; I; I=Link[I])
	{
		int x,y;
		if (neighbours[I]!=parent)
		{
			f(node,neighbours[I],x,y);
			i+=min(x,y);
			n+=x;
		}
	}
}
int in()
{
	char c;
	int x=0;
	for(;;)
	{
		c=getchar_unlocked();
		if (c<=32) return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
void add(int s,int t)
{
	if (!first[s])
		first[s]=last[s]=top;
	else
		last[s]=Link[last[s]]=top;
	neighbours[top++]=t;
}
int main()
{
	int N,i,u,v;
	N=in();
	for (i=0; i<N-1; i++)
	{
		u=in()-1; v=in()-1;
		add(u,v);
		add(v,u);
	}
	int x;
	int y;
	f(-1,0,x,y);
	printf("%d\n",min(x,y));
	return 0;
}
