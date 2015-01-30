// 2008-08-04
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <queue>
using namespace std;
int deg[10000];
static int adj[10000][50];
bool prime[10000];
void sieve()
{
	memset(prime,true,sizeof(prime));
	int i,j;
	for (i=2; i<10000; i++)
	{
		while (i<10000&&!prime[i])
			i++;
		if (i==10000)
			return;
		for (j=2*i; j<10000; j+=i)
			prime[j]=false;
	}
}
void build()
{
	int i,j;
	for (i=1000; i<10000; i++)
	{
		if (!prime[i])
			continue;
		deg[i]=0;
		int d1=i/1000;
		int d2=i/100%10;
		int d3=i/10%10;
		int d4=i%10;
		for (j=1; j<10; j++)
			if (prime[1000*j+100*d2+10*d3+d4])
				adj[i][deg[i]++]=1000*j+100*d2+10*d3+d4;
		for (j=0; j<10; j++)
			if (prime[1000*d1+100*j+10*d3+d4])
				adj[i][deg[i]++]=1000*d1+100*j+10*d3+d4;
		for (j=0; j<10; j++)
			if (prime[1000*d1+100*d2+10*j+d4])
				adj[i][deg[i]++]=1000*d1+100*d2+10*j+d4;
		for (j=0; j<10; j++)
			if (prime[1000*d1+100*d2+10*d3+j])
				adj[i][deg[i]++]=1000*d1+100*d2+10*d3+j;
	}
}
typedef pair<int,int> pii;
bool here[10000];
int main()
{
	sieve();
	build();
	int x,y,t,i;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d %d",&x,&y);
		memset(here,false,sizeof(here));
		queue<pii> Q;
		Q.push(pii(x,0));
		pii p;
		while (!Q.empty())
		{
			p=Q.front();
			Q.pop();
			if (here[p.first])
				continue;
			here[p.first]=true;
			if (p.first==y)
				break;
			for (i=0; i<deg[p.first]; i++)
				Q.push(pii(adj[p.first][i],p.second+1));
		}
		if (p.first==y)
			printf("%d\n",p.second);
		else
			printf("Impossible\n");
	}
	return 0;
}
