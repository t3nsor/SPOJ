// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;
const int dx[4]={1,0,-1, 0};
const int dy[4]={0,1, 0,-1};
int main()
{
	int T,i,j,X,Y;
	char s[1000];
	char s2[5000];
	char grid[500][500];
	scanf("%d",&T);
	for (i=1; i<=T; i++)
	{
		printf("%d ",i);
		scanf("%d %d",&Y,&X);
		getchar();
		gets(s);
		memset(grid,0,sizeof(grid));
		for (j=0; j<strlen(s); j++)
		{
			int num=s[j]-' '?s[j]-64:0;
			s2[5*j]=num>>4;
			s2[5*j+1]=(num>>3)&1;
			s2[5*j+2]=(num>>2)&1;
			s2[5*j+3]=(num>>1)&1;
			s2[5*j+4]=num&1;
		}
		for (j=5*strlen(s); j<X*Y; j++)
			s2[j]=0;
		int x=0;
		int y=0;
		int dir=0;
		for (j=0; j<X*Y; j++)
		{
			grid[y][x]=s2[j]+'0';
			if (x+dx[dir]<0||x+dx[dir]>=X||y+dy[dir]<0||y+dy[dir]>=Y||grid[y+dy[dir]][x+dx[dir]])
				dir=(dir+1)%4;
			x+=dx[dir];
			y+=dy[dir];
		}
		for (j=0; j<Y; j++)
			printf("%s",grid+j);
		putchar('\n');
	}
	return 0;
}
