// 2009-03-01
#include <iostream>
using namespace std;
int dx[]={0,0,1,1,-1,-1};
int dy[]={1,-1,1,0,0,-1};
int main()
{
	static int dp[15][40][40]={0}; //# steps; x coord; y coord
	int i,j,k,m;
	dp[0][20][20]=1;
	for (i=1; i<=14; i++)
		for (j=1; j<39; j++)
			for (k=1; k<39; k++)
				for (m=0; m<6; m++)
					dp[i][j][k]+=dp[i-1][j+dx[m]][k+dy[m]];
	scanf("%d",&i);
	while (i--)
	{
		scanf("%d",&j);
		printf("%d\n",dp[j][20][20]);
	}
	return 0;
}
