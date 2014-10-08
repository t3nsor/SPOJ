// 2010-02-13
#include <iostream>
#include <vector>
using namespace std;
int pre;
int id[10000];
int top[10000];
int loc[10000];
int pos[10000];
int size[10000];
int depth[10000];
vector<vector<int> > V;
pair<int,int> parent[10000];
pair<int,int> depth2[20001];
enum {heavy,light} color[10000];
vector<pair<int,int> > adj[10000];
pair<pair<int,int>,int> edges[10000];
void dfs1(int x,int p=-1,int d=0)
{
	depth[x]=d;
	depth2[pre]=make_pair(d,x);
	pos[x]=pre++;
	size[x]=1;
	int i;
	for (i=0; i<adj[x].size(); i++)
	{
		if (adj[x][i].first==p)
			continue;
		parent[adj[x][i].first]=make_pair(x,adj[x][i].second);
		dfs1(adj[x][i].first,x,d+1);
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
		id[x]=V.size();
		V.push_back(vector<int>(1,parent[x].second));
	}
	else
	{
		top[x]=top[parent[x].first];
		loc[x]=loc[parent[x].first]+1;
		id[x]=id[parent[x].first];
		V[id[x]].push_back(parent[x].second);
	}
	int i;
	for (i=0; i<adj[x].size(); i++)
		if (adj[x][i].first!=parent[x].first)
			dfs2(adj[x][i].first);
}
struct segtree
{
	int size;
	vector<int> data;
	template<class R_A_I>
	void construct(int idx,R_A_I begin,R_A_I end)
	{
		if (data.size()<=idx) data.resize(idx+1);
		int sz=end-begin;
		if (sz==1)
			data[idx]=*begin;
		else
		{
			R_A_I mid=begin+sz/2;
			construct(2*idx+1,begin,mid);
			construct(2*idx+2,mid,end);
			data[idx]=max(data[2*idx+1],data[2*idx+2]);
		}
	}
	int query(int x,int y,int idx=0,int l=0,int r=-1)
	{
		if (r==-1) r=size;
		if (l>=x&&r<=y)
			return data[idx];
		else
		{
			int m=(l+r)/2;
			int res=-2e9;
			if (m>x&&l<y)
				res=max(res,query(x,y,2*idx+1,l,m));
			if (r>x&&m<y)
				res=max(res,query(x,y,2*idx+2,m,r));
			return res;
		}
	}
	void update(int x,int val,int idx=0,int l=0,int r=-1)
	{
		if (r==-1) r=size;
		if (r-l==1) //done!
			data[idx]=val;
		else
		{
			int m=(l+r)/2;
			if (x<m)
				update(x,val,2*idx+1,l,m);
			else
				update(x,val,2*idx+2,m,r);
			data[idx]=max(data[2*idx+1],data[2*idx+2]);
		}
	}
	template<class R_A_I>
	segtree(R_A_I begin,R_A_I end)
	{
		size=end-begin;
		construct(0,begin,end);
	}
};
vector<segtree> S;
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
		V.clear();
		S.clear();
		pre=0;
		for (i=0; i<N-1; i++)
		{
			a=ini()-1; b=ini()-1; c=ini();
			adj[a].push_back(make_pair(b,c));
			adj[b].push_back(make_pair(a,c));
			edges[i+1]=make_pair(make_pair(a,b),c);
		}
		dfs1(0);
		color[0]=light;
		parent[0]=make_pair(-1,0);
		sparse_table_gen();
		dfs2(0);
		for (i=0; i<V.size(); i++)
			S.push_back(segtree(V[i].begin(),V[i].end()));
		char s[10];
		for (;;)
		{
			ins(s);
			if (s[0]=='D') break;
			a=ini(); b=ini();
			if (s[0]=='C')
			{
				int x=edges[a].first.first;
				int y=edges[a].first.second;
				if (x==parent[y].first)
					S[id[y]].update(loc[y],b);
				else
					S[id[x]].update(loc[x],b);
			}
			else //query
			{
				a--; b--;
				int l=RMQ(pos[a],pos[b]).second;
				int res=-2e9;
				while (id[a]!=id[l])
				{
					res=max(res,
					  S[id[a]].query(0,loc[a]+1));
					a=top[a];
				}
				res=max(res,S[id[a]].query(loc[l]+1,loc[a]+1));
				while (id[b]!=id[l])
				{
					res=max(res,
					  S[id[b]].query(0,loc[b]+1));
					b=top[b];
				}
				res=max(res,S[id[b]].query(loc[l]+1,loc[b]+1));
				printf("%d\n",res);
			}
		}
	}
	return 0;
}
