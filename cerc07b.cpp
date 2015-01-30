// 2009-09-26
#include <cstdio>
#include <set>
#include <cstring>
using namespace std;
bool a[20][20];
bool b[20][20];
void flip(int x,int y)
{
	b[x][y]=!b[x][y];
	b[x+1][y]=!b[x+1][y];
	b[x][y+1]=!b[x][y+1];
	if (x>0)
		b[x-1][y]=!b[x-1][y];
	if (y>0)
		b[x][y-1]=!b[x][y-1];
}
int main()
{
	int X,Y,i,j,k;
	for (;;)
	{
		scanf("%d %d",&Y,&X);
		if (X==0&&Y==0) break;
		char s[20];
		for (i=0; i<Y; i++)
		{
			scanf("%s",&s);
			for (j=0; j<X; j++)
				if (s[j]=='X')
					a[j][i]=true;
				else
					a[j][i]=false;
		}
		int minflips=1000;
		for (i=0; i<(1<<X); i++)
		{
			memcpy(b,a,sizeof(b));
			int flips=0;
			for (j=0; j<X; j++)
				if ((1<<j)&i)
				{
					flips++;
					flip(j,0);
				}
			for (j=1; j<Y; j++)
				for (k=0; k<X; k++)
					if (b[k][j-1])
					{
						flip(k,j);
						flips++;
					}
			bool B=true;
			for (j=0; j<X; j++)
				if (b[j][Y-1])
				{
					B=false;
					break;
				}
			if (B)
				if (flips<minflips)
					minflips=flips;
		}
		if (minflips<1000)
			printf("You have to tap %d tiles.\n",minflips);
		else
			printf("Damaged billboard.\n");
	}
	return 0;
}
