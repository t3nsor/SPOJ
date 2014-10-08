// 2010-06-20
//Kosaraju.
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>
using namespace std;
stack<int> S;
bool vis[100000]={0};
bool bad[100000]={0};
int sc[100000];
vector<int> adj[100000];
vector<int> tr[100000];
int cnt=0;
int scnt=0;
void dfs(int x)
{
    if (vis[x]) return;
    vis[x]=true;
    for (int i=0; i<adj[x].size(); i++)
	dfs(adj[x][i]);
//    cout << x << endl;
    S.push(x);
}
void dfs2(int x)
{
    if (~sc[x]) return;
    sc[x]=scnt;
    for (int i=0; i<tr[x].size(); i++)
	dfs2(tr[x][i]);
}
int main()
{
    int V,E,x,y,i,j;
    scanf("%d %d",&V,&E);
    while (E--)
    {
	scanf("%d %d",&x,&y);
	adj[x-1].push_back(y-1);
	tr[y-1].push_back(x-1);
    }
    for (i=0; i<V; i++)
	dfs(i);
    memset(sc,-1,sizeof(sc));
    while (!S.empty())
    {
	if (sc[S.top()]==-1)
	{
	    dfs2(S.top());
	    scnt++;
	}
	S.pop();
    }
//DEBUG
//    for (i=0; i<V; i++)
//	printf("sc[%d]=%d\n",i,sc[i]);
    
    for (i=0; i<V; i++)
	for (j=0; j<adj[i].size(); j++)
	    if (sc[adj[i][j]]!=sc[i])
		bad[sc[i]]=true;
    int ok=-1;
//    printf("scnt=%d\n",scnt);
    for (i=0; i<scnt; i++)
	if (!bad[i])
	    if (~ok)
	    {
		printf("0\n");
		return 0;
	    }
	    else
		ok=i;
    //cout << ok << endl;
    int c=0;
    for (i=0; i<V; i++)
	if (sc[i]==ok)
	    c++;
    printf("%d\n",c);
    for (i=0; i<V; i++)
	if (sc[i]==ok)
	    printf("%d ",i+1);
    printf("\n");
    return 0;
}
