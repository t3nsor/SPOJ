// 2008-08-05
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int main()
{
	int xwin=0;
	int owin=0;
	int t,X,Y,K,i,j,k;
	char board[200][200];
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d %d %d\n",&X,&Y,&K);
		for (i=0; i<Y; i++)
			gets(board[i]);
		bool xw=false;
		bool ow=false;
		for (i=0; i<Y; i++)
			for (j=0; j<=X-K; j++)
			{
				bool b1=true,b2=true;
				for (k=j; k<j+K; k++)
				{
					if (board[i][k]-'x')
						b1=false;
					if (board[i][k]-'o')
						b2=false;
				}
				if (b1) xw=true;
				if (b2) ow=true;
			}
		for (i=0; i<X; i++)
			for (j=0; j<=Y-K; j++)
			{
				bool b1=true,b2=true;
				for (k=j; k<j+K; k++)
				{
					if (board[k][i]-'x')
						b1=false;
					if (board[k][i]-'o')
						b2=false;
				}
				if (b1) xw=true;
				if (b2) ow=true;
			}
		for (i=0; i<=X-K; i++)
			for (j=0; j<=Y-K; j++)
			{
				bool b1=true,b2=true;
				for (k=0; k<K; k++)
				{
					if (board[j+k][i+k]-'x')
						b1=false;
					if (board[j+k][i+k]-'o')
						b2=false;
				}
				if (b1) xw=true;
				if (b2) ow=true;
			}
		for (i=0; i<=X-K; i++)
			for (j=K-1; j<Y; j++)
			{
				bool b1=true,b2=true;
				for (k=0; k<K; k++)
				{
					if (board[j-k][i+k]-'x')
						b1=false;
					if (board[j-k][i+k]-'o')
						b2=false;
				}
				if (b1) xw=true;
				if (b2) ow=true;
			}
		if (xw)
			xwin++;
		if (ow)
			owin++;
	}
	printf("%d:%d\n",xwin,owin);
	return 0;
}
