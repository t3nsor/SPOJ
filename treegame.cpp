// 2009-04-14
#include <stdio.h>
#define _(x) ((x)%2?(x)-1:(x)+1)
int in()
{
	int x=0;
	char c;
	for(;;)
	{
		c=getchar_unlocked();
		if (c<=32) return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
int main()
{
	static int tree[16][32768];
	int h,i,H,n,m;
	h=in();
	for (H=0; H<=h; H++)
		for (i=0; i<(1<<H); i++)
			tree[H][i]=-1;
	for (i=0; i<(1<<h)-1; i++)
	{
		n=in()-1;
		m=n; H=h;
		while (tree[H][_(m)]>=0)
			H--,m/=2;
		if ((h-H)%2)
			tree[h][n]=0;
		else
			tree[h][n]=1;
		putchar_unlocked(tree[h][n]+'0'); putchar_unlocked('\n');
		m=n; H=h;
		while (tree[H][_(m)]>=0)
			tree[H-1][m/2]=!tree[H],H--,m/=2;
	}
	//scanf("%d",&n);
	return 0;
} 
