// 2008-08-25
// modified 2014-10-07
#include <iostream>
#include <stack>
using namespace std;
short deg[1000];
static int adj[1000][1000];
short pre[1000];
short id[1000];
bool ok[1000];
int cnt;
int scnt;
stack<int> S;
stack<int> path;
void scR(int v)
{
	pre[v]=cnt++;
	S.push(v);
	path.push(v);
	int i,w;
	for (i=0; i<deg[v]; i++)
	{
		w=adj[v][i];
		if (pre[w]==-1)
			scR(w);
		else if (id[w]==-1)
			while (pre[path.top()]>pre[w])
				path.pop();
	}
	if (path.top()==v) path.pop(); else return;
	do
		id[w=S.top(),S.pop(),w]=scnt;
	while (w!=v);
	scnt++;
}
int in()
{
	char c;
	int x=0;
	for(;;)
	{
		c=getchar_unlocked();
		if (c<'0'||c>'9')
			return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
int main()
{
	int t,N,i,j,M,x;
	t=in();
	while (t--)
	{
		N=in();
		memset(pre,-1,2*N);
		memset(id,-1,2*N);
		memset(deg,0,2*N);
		for (i=0; i<N; i++)
		{
			M=in();
			for (j=0; j<M; j++)
			{
				x=in();
				x--;
				adj[x][deg[x]++]=i;
			}
		}
		cnt=0;
		scnt=0;
		for (i=0; i<N; i++)
			if (pre[i]==-1)
				scR(i);
		memset(ok,true,scnt);
		for (i=0; i<N; i++)
			for (j=0; j<deg[i]; j++)
				if (id[adj[i][j]]!=id[i])
					ok[id[adj[i][j]]]=false;
		int _cnt=0;
		int res;
		for (i=0; i<scnt; i++)
			if (ok[i])
			{
				_cnt++;
				res=i;
			}
		if (_cnt>1)
			printf("0\n");
		else
		{
			_cnt=0;
			for (i=0; i<N; i++)
				if (id[i]==res)
					_cnt++;
			printf("%d\n",_cnt);
		}
	}
	return 0;
}
