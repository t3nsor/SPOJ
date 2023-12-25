// 2008-08-14
// updated 2023-12-25 for legibility
#include <iostream>
using namespace std;
int BIT[1048576];
int N;
int query(int x,int y)
{
	int res=0; int z;
	for(;x>=0;x=(x&(x+1))-1)
		for (z=y;z>=0;z=(z&(z+1))-1)
			res+=BIT[x+(z<<10)];
	return res;
}
int input()
{
	int x=0;
	bool neg=false;
	char c;
	for(;;)
	{
		c=getchar_unlocked();
		if (c==' '||c=='\n'||c==EOF)
			return neg?-x:x;
		if (c=='-')
			neg=true;
		else
			x=(x<<1)+(x<<3)+c-'0';
	}
	return x;
}
char D[20];
void output(int x)
{
	if (x<0)
	{
		putchar_unlocked('-');
		x=-x;
	}
	int d=0;
	while (x)
	{
		D[d++]=x%10+'0';
		x/=10;
	}
	if (!d)
		putchar_unlocked('0');
	while (d)
		putchar_unlocked(D[--d]);
	putchar_unlocked('\n');
}
int main()
{
	int N,x,y,num,res,diff,x1,x2,y1,y2,X,Y;
	char c;
	getchar_unlocked(); getchar_unlocked();
	N=input();
	int i;
	for(;;)
	{
		c=getchar_unlocked(); getchar_unlocked();
		if (c=='3') break;
		if (c=='1')
		{
			x=input(); y=input(); num=input();
			for(X=x;X<N;X|=X+1)
				for (Y=y;Y<N;Y|=Y+1)
					BIT[X+(Y<<10)]+=num;
		}
		else
		{
			x1=input(); y1=input(); x2=input(); y2=input();
			output(query(x2,y2)-query(x1-1,y2)-query(x2,y1-1)+query(x1-1,y1-1));
		}
	}
	return 0;
}
