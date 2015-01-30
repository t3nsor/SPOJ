// 2008-06-20
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#define PC putchar
#else
#define GC getchar_unlocked
#define PC putchar_unlocked
#endif
int dx[6]={1, 1, 0,-1,-1,0};
int dy[6]={0,-1,-1, 0, 1,1};
void fix()
{
	char c;
	do
	{
		c=GC();
		if (c==-1)
			return;
	}
	while (c==' '||c=='\n');
	ungetc(c,stdin);
}
int input()
{
	fix();
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
bool jam;
char a[100][100];
int X,Y;
void dfs(int x,int y,char c)
{
	int i,x2,y2;
	if (c=='(')
	{
		if (a[x][y]==')')
		{
			jam=true;
			return;
		}
		else if (a[x][y]=='(')
			return;
		else
		{
			a[x][y]='(';
			for (i=0; i<6; i++)
			{
				x2=x+dx[i]; y2=y+dy[i];
				if (x2>=0&&x2<X&&y2>=0&&y2<Y&&a[x2][y2]!='.')
				{
					dfs(x2,y2,')');
					if (jam)
						return;
				}
			}
		}
	}
	else
	{
		if (a[x][y]=='('||a[x][y]=='I')
		{
			jam=true;
			return;
		}
		else if (a[x][y]==')')
			return;
		else
		{
			a[x][y]=')';
			for (i=0; i<6; i++)
			{
				x2=x+dx[i]; y2=y+dy[i];
				if (x2>=0&&x2<X&&y2>=0&&y2<Y&&a[x2][y2]!='.')
				{
					dfs(x2,y2,'(');
					if (jam)
						return;
				}
			}
		}
	}
}
void dfsjam(int x,int y)
{
	a[x][y]='B';
	int i,x2,y2;
	for (i=0; i<6; i++)
	{
		x2=x+dx[i]; y2=y+dy[i];
		if (x2>=0&&x2<X&&y2>=0&&y2<Y&&a[x2][y2]!='.'&&a[x2][y2]!='B')
			dfsjam(x2,y2);
	}
}
int main()
{
	int i,j;
	for(;;)
	{
		Y=input();
		X=input();
		if (X==0)
			return 0;
		PC('\n');
		for (j=0; j<Y; j++)
		{
			for (i=0; i<X; i++)
				a[i][j]=GC();
			GC();
		}
		for (i=0; i<X; i++)
			for (j=0; j<Y; j++)
				if (a[i][j]=='I')
				{
					jam=false;
					dfs(i,j,'(');
					if (jam)
						dfsjam(i,j);
				}
		for (j=0; j<Y; j++)
		{
			for (i=0; i<X; i++)
				if (a[i][j]=='*')
					PC('F');
				else
					PC(a[i][j]);
			PC('\n');
		}		
	}
}
