// 2009-05-15
#include <iostream>
using namespace std;
bool vis[20][20];
int R,C;
int res;
int vx[4]={0,1,0,-1};
int vy[4]={1,0,-1,0};
int mx[8]={0,0,1,1,1,-1,-1,-1};
int my[8]={1,-1,1,0,-1,1,0,-1};
char a[20][20];
bool ok(int x,int y)
{
	return x>=0&&x<20&&y>=0&&y<20;
}
void dfs(int r,int c)
{
	if (!ok(r,c)) return;
	if (a[r][c]!='X') return;
	if (vis[r][c]) return;
	vis[r][c]=1;
	res+=4;
	int i;
	for (i=0; i<4; i++)
		if (ok(r+vx[i],c+vy[i])&&a[r+vx[i]][c+vy[i]]=='X')
			res--;
	for (i=0; i<8; i++)
		dfs(r+mx[i],c+my[i]);
}
int main()
{
	int r,c,i,j;
	for(;;)
	{
		scanf("%d %d %d %d",&R,&C,&r,&c);
		if (!R) return 0;
		memset(vis,0,sizeof(vis));
		memset(a,0,sizeof(a));
		for (i=0; i<R; i++)
		{
			getchar();
			for (j=0; j<C; j++)
				a[i][j]=getchar();
		}
		res=0;
		dfs(r-1,c-1);
		printf("%d\n",res);
	}
}
