// 2009-05-07
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int main()
{
	int T,V,i,u,v,d;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&V);
		vector<pair<int,int> > adj[50000];
		for (i=0; i<V-1; i++)
		{
			scanf("%d %d %d",&u,&v,&d); u--; v--;
			adj[u].push_back(make_pair(v,d));
			adj[v].push_back(make_pair(u,d));
		}
		int bestv=0,bestd;
		queue<pair<int,pair<int,int> > > Q;
		bestd=0;
		Q.push(make_pair(0,make_pair(-1,0)));
		while (!Q.empty())
		{
			pair<int,pair<int,int> > P=Q.front(); Q.pop();
			if (P.first>bestd)
			{
				bestd=P.first;
				bestv=P.second.second;
			}
			for (i=0; i<adj[P.second.second].size(); i++)
				if (adj[P.second.second][i].first!=P.second.first)
					Q.push(make_pair(P.first+adj[P.second.second][i].second,make_pair(P.second.second,adj[P.second.second][i].first)));
		}
		bestd=0;
		Q.push(make_pair(0,make_pair(-1,bestv)));
		while (!Q.empty())
		{
			pair<int,pair<int,int> > P=Q.front(); Q.pop();
			if (P.first>bestd)
			{
				bestd=P.first;
				bestv=P.second.second;
			}
			for (i=0; i<adj[P.second.second].size(); i++)
				if (adj[P.second.second][i].first!=P.second.first)
					Q.push(make_pair(P.first+adj[P.second.second][i].second,make_pair(P.second.second,adj[P.second.second][i].first)));
		}
		printf("%d\n",bestd);
	}
	return 0;
}
