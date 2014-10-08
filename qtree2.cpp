// 2010-02-14
#include <iostream>
#include <vector>
using namespace std;
int pre,cnt;
int id[10000];
int top[10000];
int loc[10000];
int pos[10000];
int size[10000];
int depth[10000];
int wdepth[10000];
vector<vector<int> > V;
vector<int> bloc[10000];
pair<int,int> parent[10000];
pair<int,int> depth2[20001];
enum {heavy,light} color[10000];
vector<pair<int,int> > adj[10000];
void dfs1(int x,int p=-1,int d=0,int wd=0)
{
	depth[x]=d;
	wdepth[x]=wd;
	depth2[pre]=make_pair(d,x);
	pos[x]=pre++;
	size[x]=1;
	int i;
	for (i=0; i<adj[x].size(); i++)
	{
		if (adj[x][i].first==p)
			continue;
		parent[adj[x][i].first]=make_pair(x,adj[x][i].second);
		dfs1(adj[x][i].first,x,d+1,wd+adj[x][i].second);
		size[x]+=size[adj[x][i].first];
		depth2[pre++]=make_pair(d,x);
	}
	for (i=0; i<adj[x].size(); i++)
	{
		if (adj[x][i].first==p)
			continue;
		if (size[adj[x][i].first]>size[x]/2)
			color[adj[x][i].first]=heavy;
		else
			color[adj[x][i].first]=light;
	}
}
void dfs2(int x)
{
	if (color[x]==light)
	{
		top[x]=parent[x].first;
		loc[x]=0;
		id[x]=cnt++;
		bloc[id[x]]=vector<int>(1,x);
	}
	else
	{
		top[x]=top[parent[x].first];
		loc[x]=loc[parent[x].first]+1;
		id[x]=id[parent[x].first];
		bloc[id[x]].push_back(x);
	}
	int i;
	for (i=0; i<adj[x].size(); i++)
		if (adj[x][i].first!=parent[x].first)
			dfs2(adj[x][i].first);
}
pair<int,int> sparse_table[15][20001];
void sparse_table_gen()
{
	int i,j;
	for (i=0; i<pre; i++)
		sparse_table[0][i]=depth2[i];
	for (i=1; i<15; i++)
		for (j=0; j<=pre-(1<<i); j++)
			sparse_table[i][j]=min(sparse_table[i-1][j],
			  sparse_table[i-1][j+(1<<(i-1))]);
}
pair<int,int> RMQ(int x,int y)
{
	if (x>y) swap(x,y);
	int z=2,l=1;
	while (z<=y-x)
	{
		z<<=1;
		l++;
	}
	z>>=1; l--;
	return min(sparse_table[l][x],sparse_table[l][y-z+1]);
}
int ini()
{
	char c;
	int res=0;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		res=(res<<1)+(res<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return res;
}
void ins(char* s)
{
	char c;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		*s++=c;
		c=getchar_unlocked();
	}
	while (c>32);
	*s=0;
}
int main()
{
	int T,N,i,a,b,c;
	T=ini();
	while (T--)
	{
		N=ini();
		for (i=0; i<N; i++)
			adj[i].clear();
		pre=cnt=0;
		for (i=0; i<N-1; i++)
		{
			a=ini()-1; b=ini()-1; c=ini();
			adj[a].push_back(make_pair(b,c));
			adj[b].push_back(make_pair(a,c));
		}
		dfs1(0);
		color[0]=light;
		parent[0]=make_pair(-1,0);
		sparse_table_gen();
		dfs2(0);
		char s[10];
		for (;;)
		{
			ins(s);
			if (s[1]=='O') break; //DONE
			a=ini()-1; b=ini()-1;
			if (s[0]=='D') //DIST
			{
				int l=RMQ(pos[a],pos[b]).second;
				printf("%d\n",wdepth[a]+wdepth[b]-2*wdepth[l]);
			}
			else //KTH
			{
				c=ini()-1;
				int l=RMQ(pos[a],pos[b]).second;
				int th=0;
				int tdist=0;
				while (th<c&&a!=l)
				{
					int next;
					int dist;
					if (id[a]==id[l])
					{
						next=l;
						dist=loc[a]-loc[l];
					}
					else
					{
						next=top[a];
						dist=loc[a]+1;
					}
					if (c-th<dist)
					{
						a=bloc[id[a]][loc[a]-(c-th)];
						th=c;
					}
					else
					{
						th+=dist;
						a=next;
						tdist+=dist;
					}
				}
				if (c==th)
				{
					printf("%d\n",a+1);
					continue;
				}
				int b2=b;
				while (id[b2]!=id[l])
				{
					tdist+=loc[b2]+1;
					b2=top[b2];
				}
				tdist+=loc[b2]-loc[l];
				c=tdist-c;
				th=0;
				a=b;
				while (th<c&&a!=l)
				{
					int next;
					int dist;
					if (id[a]==id[l])
					{
						next=l;
						dist=loc[a]-loc[l];
					}
					else
					{
						next=top[a];
						dist=loc[a]+1;
					}
					if (c-th<dist)
					{
						a=bloc[id[a]][loc[a]-(c-th)];
						th=c;
					}
					else
					{
						th+=dist;
						a=next;
					}
				}
				printf("%d\n",a+1);
			}
		}
		printf("\n");
	}
	return 0;
}
