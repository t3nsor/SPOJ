// 2008-08-28
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#ifdef ONLINE_JUDGE
#define gc getchar_unlocked
#else
#define gc getchar
#endif
using namespace std;
struct Edge
{
	int v1;
	int v2;
	int wt;
	Edge(){}
	Edge(int v,int V,int W):v1(v),v2(V),wt(W){}
};
bool operator<(Edge e1,Edge e2)
{
	return e1.wt<e2.wt;
}
#define MAXEDGES 500000
int in()
{
	char c;
	int res=0;
	for(;;)
	{
		c=gc();
		if (c<=32)
			return res;
		res=(res<<1)+(res<<3)+c-'0';
	}
}
int main()
{
	int t,i,j,V,E,n;
	t=in();
	static Edge edges[MAXEDGES];
	int id[10000];
	int sz[10000];
	while (t--)
	{
		gc();
		V=in();
		E=0;
		char s[20];
		int x,y;
		for (i=0; i<V; i++)
		{
			id[i]=i;
			sz[i]=1;
			do{}while (gc()-'\n');
			n=in();
			for (j=0; j<n; j++)
			{
				x=in();
				y=in();
				if (x-1>i)
					continue;
				edges[E].v1=i;
				edges[E].v2=x-1;
				edges[E].wt=y;
				E++;
				if (E==MAXEDGES)
					throw;
			}
		}
		sort(edges,edges+E);
		int done=0;
		unsigned int weight=0;
		for (i=0; done<V-1; i++)
		{
			int a=edges[i].v1;
			int b=edges[i].v2;
			int w=edges[i].wt;
			while (a!=id[a])
			{
				if (id[a]!=id[id[a]])
					sz[id[a]]-=sz[a];
				a=id[id[a]];
			}
			while (b!=id[b])
			{
				if (id[b]!=id[id[b]])
					sz[id[b]]-=sz[b];
				b=id[id[b]];
			}
			if (a==b)
				continue;
			else
			{
				weight+=w;
				done++;
				if (sz[a]<sz[b])
				{
					id[a]=b;
					sz[b]+=sz[a];
				}
				else
				{
					id[b]=a;
					sz[a]+=sz[b];
				}
			}
		}
		printf("%d\n",weight);
	}
	return 0;
}
