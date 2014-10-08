// 2008-06-22
#include <cstdio>
#include <queue>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
//#define INPUT_HACK
using namespace std;
int input()
{
#ifdef INPUT_HACK
	char c;
	int x=0;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
#else
	int x;
	scanf("%d",&x);
	return x;
#endif
}
struct node
{
	int v;
	int r;
	int d;
	node(){}
	node(int V,int R,int D):v(V),r(R),d(D){}
};
bool operator>(node n1,node n2)
{
	return n1.d>n2.d;
}
unsigned int minrisk[100];
int mintime[100];
int dist[100][100];
int risk[100][100];
int N,T;
void solve(int src)
{
	int i;
	priority_queue<node,vector<node>,greater<node> > PQ;
	PQ.push(node(0,0,0));
	while (!PQ.empty())
	{
		node n=PQ.top();
		PQ.pop();
		if (minrisk[n.v]<=n.r)
			continue;
		minrisk[n.v]=n.r;
		mintime[n.v]=n.d;
		for (i=0; i<N; i++)
			if (i!=n.v&&n.d+dist[n.v][i]<=T)
				PQ.push(node(i,n.r+risk[n.v][i],n.d+dist[n.v][i]));
	}
}
int main()
{
	int j,k;
	for(;;)
	{
		N=input(),T=input(); if (N==0) return 0;
		for (j=0; j<N; j++)
			for (k=0; k<N; k++)
				dist[j][k]=input();
		for (j=0; j<N; j++)
		{
			for (k=0; k<N; k++)
				risk[j][k]=input();
			minrisk[j]=-1;
		}
		solve(0);
		printf("%d %d\n",minrisk[N-1],mintime[N-1]);
	}
}
