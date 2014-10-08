// 2008-02-02
#include <stdio.h>
#include <queue>
using namespace std;
#define INF 10000000
struct point
{
	point(){}
	point(int X,int Y,int Dist)
	{
		x=X;
		y=Y;
		dist=Dist;
	}
	int x;
	int y;
	int dist;
};
int main()
{
	queue<point> Q;
	point p;
	int t,i,j,k,X,Y;
	char s[190];
	int dist[182][182];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d %d",&Y,&X);
		for (j=0; j<Y; j++)
		{
			scanf("%s",&s);
			for (k=0; k<X; k++)
			{
				dist[k][j]=INF;
				if (s[k]=='1')
					Q.push(point(k,j,0));
			}
		}
		while (!Q.empty())
		{
			p=Q.front();
			Q.pop();
			if (p.x<0) continue;
			if (p.x>=X) continue;
			if (p.y<0) continue;
			if (p.y>=Y) continue;
			if (dist[p.x][p.y]<=p.dist) continue;
			dist[p.x][p.y]=p.dist;
			Q.push(point(p.x-1,p.y,p.dist+1));
			Q.push(point(p.x,p.y-1,p.dist+1));
			Q.push(point(p.x+1,p.y,p.dist+1));
			Q.push(point(p.x,p.y+1,p.dist+1));
		}
		for (j=0; j<Y; j++)
			for (k=0; k<X; k++)
			{
				printf("%d",dist[k][j]);
				if (k==X-1) putchar('\n'); else putchar(' ');
			}
	}
	return 0;
}
