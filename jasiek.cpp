// 2008-06-13
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
int X[30000];
int Y[30000];
int x,y;
int V;
void process(char lastc,char c)
{
	if (lastc=='N')
		if (c=='E')
			X[V]=x,Y[V]=y,V++;
		else if (c=='S')
		{
			X[V]=x+1,Y[V]=y+2,V++;
			X[V]=x,Y[V]=y+2,V++;
		}
		else //c=='W'
			X[V]=x+2,Y[V]=y+1,V++;
	else if (lastc=='E')
		if (c=='N')
			X[V]=x+1,Y[V]=y-1,V++;
		else if (c=='S')
			X[V]=x,Y[V]=y+1,V++;
		else //c=='W'
		{
			X[V]=x+2,Y[V]=y,V++;
			X[V]=x+2,Y[V]=y+1,V++;
		}
	else if (lastc=='S')
		if (c=='N')
		{
			X[V]=x,Y[V]=y-1,V++;
			X[V]=x+1,Y[V]=y-1,V++;
		}
		else if (c=='E')
			X[V]=x-1,Y[V]=y,V++;
		else //c=='W'
			X[V]=x+1,Y[V]=y+1,V++;
	else //lastc=='W'
		if (c=='N')
			X[V]=x+1,Y[V]=y,V++;
		else if (c=='E')
		{
			X[V]=x-1,Y[V]=y+1,V++;
			X[V]=x-1,Y[V]=y,V++;
		}
		else //c=='S'
			X[V]=x,Y[V]=y+2,V++;
}
int main()
{
	int i,j,area;
	char c,char1,lastc;
	for (i=0; i<10; i++)
	{
		x=0; y=0; V=0;
		GC();GC();
		char1=GC(); GC();
		if (char1=='K')
		{
			printf("1\n");
			continue;
		}
		c=char1;
		for(;;)
		{
			lastc=c;
			c=GC(); GC();
			if (c=='K') break;
			if (c=='N')
				y++;
			else if (c=='E')
				x++;
			else if (c=='S')
				y--;
			else if (c=='W')
				x--;
			if (c!=lastc)
				process(lastc,c);
		}
		if (char1=='N') y++; else if (char1=='E') x++; else if (char1=='S') y--; else x--;
		if (lastc!=char1)
			process(lastc,char1);
		area=0;
		for (j=0; j<V-1; j++)
			area+=X[j]*Y[j+1]-X[j+1]*Y[j];
		area+=X[V-1]*Y[0]-X[0]*Y[V-1];
		printf("%d\n",area/2);
	}
	return 0;
}
