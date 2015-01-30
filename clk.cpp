// 2008-09-20
#include <iostream>
using namespace std;
#define num_boards 184756
#define NPosition 0
#define PPosition 1
struct board
{
	int ht[10];
	board(){memset(ht,0,sizeof(ht));}
};
void operator++(board& B,int x)
{
	int i;
	for (i=9;;i--)
	{
		B.ht[i]++;
		if (i>0&&B.ht[i]>B.ht[i-1])
			B.ht[i]=0;
		else
			return;
	}
}
inline int min(int x,int y)
{
	return x<y?x:y;
}
int analysis[num_boards];
int nCr[21][21];
//int nCrSum[21][21];
int num(board b)
{
	int res=0;
	int i,j;
	for (i=0; i<10; i++)
	{
		for (j=0; j<b.ht[i]; j++)
			res+=nCr[9-i+j][9-i];
	}
	return res;
}
int main()
{
	nCr[0][0]=1;
	board b;
	board B;
	bool _b;
	analysis[0]=NPosition; b++;
	int i,j,k,m,N;
	for (i=1; i<=20; i++)
	{
		nCr[i][0]=1;
		for (j=1; j<=i; j++)
			nCr[i][j]=nCr[i-1][j]+nCr[i-1][j-1];
	}
	for (i=1; i<num_boards; i++,b++)
	{
		_b=false;
		for (j=0; j<10&&b.ht[j]; j++)
		{
			for (k=0; k<b.ht[j]; k++)
			{
				for (m=0; m<j; m++)
					B.ht[m]=b.ht[m];
				for (m=j; m<10; m++)
					B.ht[m]=min(b.ht[m],k);
				if (analysis[num(B)]==PPosition)
				{
					_b=true;
					break;
				}
			}
			if (_b) break;
		}
		if (_b)
			analysis[num(b)]=NPosition;
		else
			analysis[num(b)]=PPosition;
	}
	scanf("%d",&N);
	for (i=0; i<N; i++)
	{
		for (j=0; j<10; j++)
			scanf("%d",b.ht+j);
		if (analysis[num(b)]==NPosition)
			putchar('W');
		else
			putchar('L');
		putchar('\n');
	}
	return 0;
}
