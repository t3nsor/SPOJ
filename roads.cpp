// 2008-06-22
#include <cstdio>
#include <queue>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
#define INPUT_HACK
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
	int t;
	int d;
	node(){}
	node(int V,int T,int D):v(V),t(T),d(D){}
};
bool operator>(node n1,node n2)
{
	return n1.t>n2.t;
}
int nadj[100];
int adjl[100][100];
int dist[100][100];
int toll[100][100];
unsigned int mindist[100];
int K;
void solve(int src)
{
	int i;
	priority_queue<node,vector<node>,greater<node> > PQ;
	PQ.push(node(0,0,0));
	while (!PQ.empty())
	{
		node n=PQ.top();
		PQ.pop();
		if (mindist[n.v]<=n.d)
			continue;
		mindist[n.v]=n.d;
		for (i=0; i<nadj[n.v]; i++)
			if (n.t+toll[n.v][i]<=K)
				PQ.push(node(adjl[n.v][i],n.t+toll[n.v][i],n.d+dist[n.v][i]));
	}
}
int main()
{
	int t,i,j,N,R,S,D,L,T;
	t=input();
	for (i=0; i<t; i++)
	{
		K=input(),N=input(),R=input();
		for (j=0; j<N; j++)
		{
			nadj[j]=0;
			mindist[j]=-1;
		}
		for (j=0; j<R; j++)
		{
			S=input(),D=input(),L=input(),T=input();S--,D--;
			adjl[S][nadj[S]]=D;
			dist[S][nadj[S]]=L;
			toll[S][nadj[S]]=T;
			nadj[S]++;
		}
		solve(0);
		printf("%d\n",mindist[N-1]);
	}
	return 0;
}
