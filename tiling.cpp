// 2008-08-03
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int t;
	scanf("%d",&t);
	static char grid[1005][1005];
	int i,j,k,R,C,rowtile,rowindex,coltile,colindex;
	while(t--)
	{
		scanf("%d %d\n",&R,&C);
		for (i=0; i<R; i++)
			gets(grid[i]);
		int fx[100]; int nfx=0;
		int fy[100]; int nfy=0;
		for (i=1; i<=R; i++)
			if (R%i==0)
				fy[nfy++]=i;
		for (i=1; i<=C; i++)
			if (C%i==0)
				fx[nfx++]=i;
		rowtile=1;
		int prev=1;
		rowindex=0;
		for (i=0; i<R; i++)
		{
			for(;;)
			{
				bool B=true;
				int ntile=C/rowtile;
				for (j=rowtile; j<C; j+=rowtile)
				{
					for (k=0; k<rowtile; k++)
						if (grid[i][j+k]!=grid[i][k])
						{
							B=false;
							break;
						}
					if (!B)
						break;
				}
				if (B)
					break;
				do
					rowindex++;
				while (fx[rowindex]%prev || C%fx[rowindex]);
				rowtile=fx[rowindex];
			}
			prev=rowtile;
		}
		coltile=1;
		prev=1;
		colindex=0;
		for (i=0; i<C; i++)
		{
			for(;;)
			{
				bool B=true;
				int ntile=R/coltile;
				for (j=coltile; j<R; j+=coltile)
				{
					for (k=0; k<coltile; k++)
						if (grid[j+k][i]!=grid[k][i])
						{
							B=false;
							break;
						}
					if (!B)
						break;
				}
				if (B)
					break;
				do
					colindex++;
				while (fy[colindex]%prev || R%fy[colindex]);
				coltile=fy[colindex];
			}
			prev=coltile;
		}
		printf("%d\n",rowtile*coltile);
	}
	return 0;
}
