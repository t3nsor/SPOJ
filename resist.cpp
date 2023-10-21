// 2023-10-21
// originally written 2010-01-10 but with compilation errors
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
int N; //size of matrix
long double M1[100][100];
long double v1[100];
vector<pair<int,int> > adj;
void row_swap(int r1,int r2)
{
	int i;
	for (i=0; i<N; i++)
		swap(M1[r1][i],M1[r2][i]);
	swap(v1[r1],v1[r2]);
}
void row_scale(int r1,int r2,long double w1,long double w2)
{
	int i;
	for (i=0; i<N; i++)
		M1[r1][i]=w1*M1[r1][i]+w2*M1[r2][i];
	v1[r1]=w1*v1[r1]+w2*v1[r2];
}
void solve()
{
	int i,j;
	for (j=0; j<N; j++) //column
	{
		//make M[j][j] equal to 1
		if (fabs(M1[j][j])<1e-12) //find one that's not
		{
			bool ok=false;
			for (i=0; i<N; i++)
				if (fabs(M1[i][j])>1e-12)
				{
					ok=true;
					row_swap(i,j);
					break;
				}
			if (!ok) throw;
		}
		row_scale(j,0,1.0/M1[j][j],0.0);
		for (i=0; i<N; i++)
		{
			if (i==j) continue;
			row_scale(i,j,1.0,-M1[i][j]);
		}
	}
}
int main()
{
	int M,i,j;
	while (~scanf("%d %d",&N,&M))
	{
		memset(M1,0,sizeof(M1));
		memset(v1,0,sizeof(v1));
		adj.clear();
		while (M--)
		{
			int x,y,r;
			scanf("%d %d %d",&x,&y,&r);
			x--; y--;
			if (x>y) swap(x,y);
			if (x==0)
				adj.push_back(make_pair(y,r));
			else // (x!=0)
			{
				M1[x][x]-=1.0/r;
				M1[x][y]+=1.0/r;
			}
			if (y!=N-1)
			{
				M1[y][x]+=1.0/r;
				M1[y][y]-=1.0/r;
			}
		}
		M1[0][0]=1;
		v1[0]=1;
		M1[N-1][N-1]=1;
		solve();
		double total=0;
		for (i=0; i<adj.size(); i++)
			total+=(1.0-v1[adj[i].first])/adj[i].second;
		printf("%.2f\n",1.0/total);
	}
	return 0;
}
