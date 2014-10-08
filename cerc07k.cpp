// 2008-10-01
#include <cstdio>
#include <queue>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
using namespace std;
struct QueueNode
{
	int x;
	int y;
	int mask; //BGYR
	int moves;
	QueueNode(){}
	QueueNode(int X,int Y,int Mask,int Moves):x(X),y(Y),mask(Mask),moves(Moves){}
};
void fix()
{
	char c;
	do
	{
		c=GC();
		if (c==-1) return;
	}
	while (c==' '||c=='\n');
	ungetc(c,stdin);
}
int main()
{
	bool here[100][100][16];
	int X,Y,i,j;
	char a[100][100];
	for(;;)
	{
		scanf("%d %d",&Y,&X);
		queue<QueueNode> Q;
		QueueNode QN;
		QN.mask=0;
		QN.moves=0;
		if (X==0)
			return 0;
		for (i=0; i<Y; i++)
			for (j=0; j<X; j++)
			{
				fix();
				a[j][i]=GC();
				if (a[j][i]=='*')
				{
					QN.x=j;
					QN.y=i;
					a[j][i]='.';
				}
			}
		Q.push(QN);
		memset(here,false,sizeof(here));
		bool found=false;
		while (!Q.empty())
		{
			QN=Q.front();
			Q.pop();
			if (QN.x<0) continue;
			if (QN.y<0) continue;
			if (QN.x>=X) continue;
			if (QN.y>=Y) continue;
			if (a[QN.x][QN.y]=='#') continue;
			if (a[QN.x][QN.y]=='X')
			{
				found=true;
				break;
			}
			if (a[QN.x][QN.y]=='r')
				QN.mask|=1;
			else if (a[QN.x][QN.y]=='y')
				QN.mask|=2;
			else if (a[QN.x][QN.y]=='g')
				QN.mask|=4;
			else if (a[QN.x][QN.y]=='b')
				QN.mask|=8;
			if (here[QN.x][QN.y][QN.mask])
				continue;
			if (a[QN.x][QN.y]=='R'&&!(QN.mask&1))
				continue;
			if (a[QN.x][QN.y]=='Y'&&!(QN.mask&2))
				continue;
			if (a[QN.x][QN.y]=='G'&&!(QN.mask&4))
				continue;
			if (a[QN.x][QN.y]=='B'&&!(QN.mask&8))
				continue;
			here[QN.x][QN.y][QN.mask]=true;
			Q.push(QueueNode(QN.x-1,QN.y,QN.mask,QN.moves+1));
			Q.push(QueueNode(QN.x+1,QN.y,QN.mask,QN.moves+1));
			Q.push(QueueNode(QN.x,QN.y-1,QN.mask,QN.moves+1));
			Q.push(QueueNode(QN.x,QN.y+1,QN.mask,QN.moves+1));
		}
		if (!found)
			printf("The poor student is trapped!\n");
		else
			printf("Escape possible in %d steps.\n",QN.moves);
	}
}
