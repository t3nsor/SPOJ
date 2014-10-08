// 2009-03-09
#include <cstdio>
int input()
{
	int x=0;
	char c;
	do
	{
		c=getchar_unlocked(); 
	}
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int next[50000];
int eat[50000];
int main()
{
	int t,i,j,n,k,f,D,X,Y,rX,rY,eX,eY;
	t=input();
	for (i=0; i<t; i++)
	{
		n=input();
		f=0;
		for (j=0; j<n; j++)
		{
			next[j]=j;
			eat[j]=0;
		}
		k=input();
		for (j=0; j<k; j++)
		{
			D=input();
			X=input();
			Y=input();
			if (X>n||Y>n)
			{
				f++;
				continue;
			}
			X--,Y--;
			rX=X; rY=Y;
			eX=0; eY=0;
			while (next[rX]!=rX)
			{
				eat[rX]+=eat[next[rX]];
				eX+=eat[rX];
				rX=next[rX]=next[next[rX]];
			}
			eX%=3;
			if (eX<0)
				throw;
			while (next[rY]!=rY)
			{
				eat[rY]+=eat[next[rY]];
				eY+=eat[rY];
				rY=next[rY]=next[next[rY]];
			}
			eY%=3;
			if (eY<0)
				throw;
			if (rX==rY)
			{
				if (D==1&&eX!=eY)
					f++;
				else if (D==2&&(eX-eY+3)%3!=1)
					f++;
			}
			else
			{
				next[rX]=rY;
				if (D==1)
					eat[rX]=(eY-eX+3)%3;
				else
					eat[rX]=(eY-eX+4)%3;
			}
		}
		printf("%d\n",f);
	}
	return 0;
}
