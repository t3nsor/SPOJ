// 2008-07-24
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
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
int main()
{
	int t,i,j,k,p,V,E;
	scanf("%d",&t);
	static unsigned int length[1000][1000];
	static Edge edges[300000];
	int id[1000];
	int sz[1000];
	for (i=0; i<t; i++)
	{
		memset(length,-1,sizeof(length));
		scanf("%d %d %d",&p,&V,&E);
		for (j=0; j<V; j++)
		{
			id[j]=j;
			sz[j]=1;
		}
		for (j=0; j<E; j++)
		{
			int V1,V2,w;
			scanf("%d %d %d",&V1,&V2,&w);
			V1--,V2--;
			if (V1<V2)
				swap(V1,V2);
			if (length[V1][V2]>w)
				length[V1][V2]=w;
		}
		E=0;
		for (j=0; j<V; j++)
			for (k=0; k<j; k++)
				if (length[j][k]!=-1)
					edges[E++]=Edge(j,k,length[j][k]);
		sort(edges,edges+E);
		int done=0;
		unsigned int weight=0;
		for (j=0; done<V-1; j++)
		{
			int a=edges[j].v1;
			int b=edges[j].v2;
			int w=edges[j].wt;
			while (a!=id[a])
				a=id[a];
			while (b!=id[b])
				b=id[b];
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
		printf("%d\n",weight*p);
	}
	return 0;
}
