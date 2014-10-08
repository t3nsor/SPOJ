// 2008-06-21
#include <cstdio>
#include <queue>
using namespace std;
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
int main()
{
	int T,i,x,y,X,Y,z;
	static char a[1000][1000];
	static bool visited[1000][1000];
	static bool visited2[1000][1000];
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		scanf("%d %d",&X,&Y);
		for (y=0; y<Y; y++)
		{
			for (x=0; x<X; x++)
			{
				a[x][y]=GC();
				visited[x][y]=false;
				visited2[x][y]=false;
			}
			GC();
		}
		int n;
		for (x=0; x<X; x++)
		{
			for (y=0; y<Y; y++)
			{
				if (a[x][y]=='#')
					continue;
				n=0;
				if (x>0&&a[x-1][y]=='.')
					n++;
				if (x<X-1&&a[x+1][y]=='.')
					n++;
				if (y>0&&a[x][y-1]=='.')
					n++;
				if (y<Y-1&&a[x][y+1]=='.')
					n++;
				if (n==1) //corner
					break;
			}
			if (n==1)
				break;
		}
		if (x==X&&y==Y)
		{
			printf("Maximum rope length is 0.\n");
			continue;
		}
		queue<int> qx;
		queue<int> qy;
		qx.push(x);
		qy.push(y);
		int x2,y2,z2;
		while (!qx.empty())
		{
			x=qx.front(),y=qy.front();
			qx.pop(),qy.pop();
			if (visited[x][y])
				continue;
			visited[x][y]=true;
			if (x>0&&a[x-1][y]=='.')
				qx.push(x-1),qy.push(y);
			if (x<X-1&&a[x+1][y]=='.')
				qx.push(x+1),qy.push(y);
			if (y>0&&a[x][y-1]=='.')
				qx.push(x),qy.push(y-1);
			if (y<Y-1&&a[x][y+1]=='.')
				qx.push(x),qy.push(y+1);
			x2=x,y2=y;
		}
		qx.push(x2);
		qy.push(y2);
		queue<int> qz;
		qz.push(0);
		while (!qx.empty())
		{
			x=qx.front(),y=qy.front(),z=qz.front();
			qx.pop(),qy.pop(),qz.pop();
			if (visited2[x][y])
				continue;
			visited2[x][y]=true;
			if (x>0&&a[x-1][y]=='.')
				qx.push(x-1),qy.push(y),qz.push(z+1);
			if (x<X-1&&a[x+1][y]=='.')
				qx.push(x+1),qy.push(y),qz.push(z+1);
			if (y>0&&a[x][y-1]=='.')
				qx.push(x),qy.push(y-1),qz.push(z+1);
			if (y<Y-1&&a[x][y+1]=='.')
				qx.push(x),qy.push(y+1),qz.push(z+1);
			x2=x,y2=y,z2=z;
		}
		printf("Maximum rope length is %d.\n",z2);
	}
	return 0;
}
