// 2009-04-07
#include <iostream>
#include <vector>
using namespace std;
inline int max(int x,int y){return x>y?x:y;}
int main()
{
	int T,VMax,N,i,j,k,V[66],I[66],u[66];
	scanf("%d",&T);
	while (T--)
	{
		vector<int> attach[66];
		scanf("%d %d",&VMax,&N);
		for (i=0; i<N; i++)
		{
			scanf("%d %d %d",V+i,I+i,u+i);
			if (u[i])
				attach[u[i]-1].push_back(i);
		}
		int dp[33333]={0};
		int res=0;
		for (i=0; i<N; i++)
		{
			if (u[i]) continue;
			vector<pair<int,int> > pos;
			for (j=0; j<(1<<(attach[i].size())); j++)
			{
				int tmpi=V[i]*I[i];
				int tmpv=V[i];
				for (k=0; k<attach[i].size(); k++)
					if (j&(1<<k))
					{
						tmpi+=V[attach[i][k]]*I[attach[i][k]];
						tmpv+=V[attach[i][k]];
					}
				pos.push_back(make_pair(tmpv,tmpi));
			}
			for (j=VMax; j>=0; j--)
				for (k=0; k<pos.size(); k++)
					if (j>=pos[k].first)
						res=max(res,dp[j]=max(dp[j],dp[j-pos[k].first]+pos[k].second));
		}
		printf("%d\n",res);
	}
	return 0;
}
