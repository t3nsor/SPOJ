/* 2009-04-19 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
char* opos;
char* ipos;
char infile[20000000];
char outfile[10000000];
int BITPool[20971520];
int arrPool[20971520];
int* BIT;
int* arr;
int N,cnt;
int* p;
int query(int x,int y)
{
	int res=0; int z;
	for(;x>=0;x=(x&(x+1))-1)
	{
		p=BIT+x*N;
		for (z=y;z>=0;z=(z&(z+1))-1)
			res+=p[z];
	}
	return res;
}
int query_experimental(int x,int y1,int y2)
{
	int res=0; int z;
	for(;x>=0;x=(x&(x+1))-1)
	{
		for (z=y1; z>=0; z=(z&(z+1))-1)
			res+=BIT[(x<<cnt)+z];
		for (z=y2; z>=0; z=(z&(z+1))-1)
			res-=BIT[(x<<cnt)+z];
	}
	return res;
}
int input()
{
	int x=0;
	int neg=0;
	char c;
	for(;;)
	{
		c=*ipos++;
		if (c<=32)
			return neg?-x:x;
		if (c=='-')
			neg=1;
		else
			x=(x<<1)+(x<<3)+c-'0';
	}
	return x;
}
char D[20];
void output(int x)
{
	int y,dig=0;
	if (x<0)
	{
		*opos++='-';
		x=-x;
	}
	while (x||!dig)
	{
		y=x/10;
		D[dig++]=x-(10*y)+'0';
		x=y;
	}
	while (dig--)
		*opos++=D[dig];
	*opos++='\n';
}
int main()
{
	ipos=infile;
	opos=outfile;
	int T,x,y,num,res,diff,x1,x2,y1,y2,X,Y,sum,i,j;
	char c;
	fread_unlocked(infile,20000000,1,stdin);
	T=input();
	assert (T<=20);
	BIT=BITPool;
	arr=arrPool;
	while (T--)
	{
		N=input();
		for(;;)
		{
			ipos+=2; c=*ipos++; ipos++;
			if (c=='D') break;
			if (c=='T')
			{
				x=input(); y=input(); num=input();
				res=0;
				diff=num-arr[x*N+y];
				for(X=x;X<N;X|=X+1)
				{
					p=BIT+X*N;
					for (Y=y;Y<N;Y|=Y+1)
						p[Y]+=diff;
				}
				arr[x*N+y]=num;
			}
			else
			{
				x1=input(); y1=input(); x2=input(); y2=input();
				output(query(x2,y2)-query(x1-1,y2)-query(x2,y1-1)+query(x1-1,y1-1));
			}
		}
		BIT+=1048576;
		arr+=1048576;
	}
	fwrite_unlocked(outfile,opos-outfile,1,stdout);
	return 0;
}

