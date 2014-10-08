// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;
const int dx[4]={1,0,-1, 0};
const int dy[4]={0,1, 0,-1};
int main()
{
	int T,i,j,X,Y,k,m,x,y,dir;
	char s[5000];
	char s2[1000];
	char grid[500][500];
	char res[1000];
	scanf("%d",&T);
	for (i=1; i<=T; i++)
	{
		printf("%d ",i);
		scanf("%d %d",&Y,&X);
		getchar();
		gets(s);
		memset(grid,0,sizeof(grid));
		for (j=0,m=0; j<Y; j++)
			for (k=0; k<X; k++,m++)
				grid[j][k]=s[m];
		for (j=x=y=dir=0; j<X*Y; j++)
		{
			s2[j]=grid[y][x]-'0';
			grid[y][x]=0;
			if (x+dx[dir]<0||x+dx[dir]>=X||y+dy[dir]<0||y+dy[dir]>=Y||!grid[y+dy[dir]][x+dx[dir]])
				dir=(dir+1)%4;
			x+=dx[dir];
			y+=dy[dir];
		}
		for (j=0; j<X*Y/5; j++)
		{
			int num=(s2[5*j]<<4)+(s2[5*j+1]<<3)+(s2[5*j+2]<<2)+(s2[5*j+3]<<1)+s2[5*j+4];
			res[j]=num?num+64:' ';
		}
		j=X*Y/5-1;
		while (j>=0&&res[j]==' ')
			j--;
		res[j+1]=0;
		printf("%s\n",res);
	}
	return 0;
}
