// 2008-06-17
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#define PC putchar
#else
#define GC getchar_unlocked
#define PC putchar_unlocked
#endif
#define ABS(x) ((x)>0?x:-(x))
#define DIST(ax,ay,bx,by) (ABS((ax)-(bx))+ABS((ay)-(by)))
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
inline void yes()
{
	PC('Y');PC('E');PC('S');PC('\n');
}
int main()
{
	int X,Y,t,i,my,mx,cy,cx,Cy,Cx;
	Y=input();
	X=input();
	t=input();
	for (i=0; i<t; i++)
	{
		my=input(); my--;
		mx=input(); mx--;
		cy=input(); cy--;
		cx=input(); cx--;
		Cy=input(); Cy--;
		Cx=input(); Cx--;
		if (mx<DIST(cx,cy,0,my)&&mx<DIST(Cx,Cy,0,my))
		{
			yes();
			continue;
		}
		if (my<DIST(cx,cy,mx,0)&&my<DIST(Cx,Cy,mx,0))
		{
			yes();
			continue;
		}
		if (X-mx-1<DIST(cx,cy,X-1,my)&&X-mx-1<DIST(Cx,Cy,X-1,my))
		{
			yes();
			continue;
		}
		if (Y-my-1<DIST(cx,cy,mx,Y-1)&&Y-my-1<DIST(Cx,Cy,mx,Y-1))
		{
			yes();
			continue;
		}
		PC('N');PC('O');PC('\n');
	}
	return 0;
}
