// 2008-06-29
#include <cstdio>
char a[100][100];
int X,Y;
void explore(int x,int y)
{
	a[x][y]='#';
	if (x>0&&a[x-1][y]=='-')
		explore(x-1,y);
	if (y>0&&a[x][y-1]=='-')
		explore(x,y-1);
	if (x<X-1&&a[x+1][y]=='-')
		explore(x+1,y);
	if (y<Y-1&&a[x][y+1]=='-')
		explore(x,y+1);
}
int main()
{
	int T,i,p,j,k,r;
	scanf("%d",&T);
	char s[200];
	for (i=0; i<T; i++)
	{
		scanf("%d %d",&Y,&X);
		p=0;
		for (j=0; j<Y; j++)
		{
			scanf("%s",&s);
			for (k=0; k<X; k++)
			{
				if (s[k]=='*')
				{
					p++;
					a[k][j]='-';
				}
				else
					a[k][j]=s[k];
			}
		}
		r=0;
		for (j=0; j<X; j++)
			for (k=0; k<Y; k++)
				if (a[j][k]=='-')
				{
					r++;
					explore(j,k);
				}
		printf("%.2lf\n",double(p)/double(r));
	}
	return 0;
}
