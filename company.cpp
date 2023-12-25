// 2010-01-10
// I can't guarantee that this solution is actually correct, since people in
// the comments mentioned that there's something wrong with the grader.
#include <iostream>
#include <vector>
#include <bitset>
using namespace std;
bitset<1000> matrix[1000];
bitset<1000> vis[1000];
vector<int> adj[1000];
vector<pair<int,int> > res;
void dfs(int v,int orig)
{
	if (vis[orig][v])
		return;
	vis[orig][v]=true;
	int i;
	for (i=0; i<adj[v].size(); i++)
		dfs(adj[v][i],orig);
}
int main()
{
	int i,j,M,N;
	scanf("%d %d",&N,&M);
	for (i=0; i<M; i++)
	{
		int x,y;
		scanf("%d %d",&x,&y);
		x--, y--;
		matrix[x][y]=true;
		adj[x].push_back(y);
	}
	for (i=0; i<N; i++)
	{
		dfs(i,i);
		vis[i][i]=false;
	}
	for (i=0; i<N; i++)
		for (j=0; j<adj[i].size(); j++)
			matrix[i]&=~vis[adj[i][j]];
	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			if (matrix[i][j])
				res.push_back(make_pair(i,j));
	printf("%d\n",res.size());
	for (i=0; i<res.size(); i++)
		printf("%d %d\n",res[i].first+1,res[i].second+1);
	return 0;
}
