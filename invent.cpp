// 2008-07-20
// Note: I'm pretty sure I cheated on this problem by looking at the solution
// on the IPSC website...
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
using namespace std;
struct Edge
{
	int s;
	int t;
	int w;
	Edge(){}
	Edge(int S,int T,int W):s(S),t(T),w(W){}
};
bool operator<(Edge e1,Edge e2)
{
	return e1.w<e2.w;
}
int main()
{
	int T,i,N,j;
	int id[15000];
	int size[15000];
	Edge E[15000];
	long long sum=0;
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		scanf("%d",&N);
		for (j=0; j<N; j++)
		{
			id[j]=j;
			size[j]=1;
		}
		for (j=0; j<N-1; j++)
		{
			scanf("%d %d %d",&E[j].s,&E[j].t,&E[j].w);
			E[j].s--;
			E[j].t--;
		}
		sort(E,E+(N-1));
		sum=0;
		for (j=0; j<N-1; j++)
		{
			int s=E[j].s;
			int t=E[j].t;
			while (s!=id[s])
			{
				if (id[s]!=id[id[s]])
					size[id[s]]-=size[s];
				//size[id[id[s]]]+=size[s];
				s=id[s]=id[id[s]];
			}
			while (t!=id[t])
			{
				if (id[t]!=id[id[t]])
					size[id[t]]-=size[t];
				t=id[t]=id[id[t]];
			}
			sum+=((long long)size[s]*size[t]-1)*(E[j].w+1)+E[j].w;
			if (size[s]>size[t])
			{
				size[s]+=size[t];
				id[t]=s;
			}
			else
			{
				size[t]+=size[s];
				id[s]=t;
			}
		}
		printf("%lld\n",sum);
	}
	return 0;
}
