// 2009-03-26
#include <stdio.h>
#include <string.h>
char infile[20000000];
char outfile[20000000];
char s[1000001];
char *ip,*op;
char c;
int x;
int in()
{
	x=0;
	for(;;)
	{
		c=*ip++;
		if (c<=32) return x;
		x=(x<<1)+(x<<3)+c-'0';
	}
}
int d;
void out(int x)
{
	if (x<10)
		*op++=x+'0';
	else
	{
		d=x/10;
		out(d);
		d=(d<<1)+(d<<3);
		*op++=x-d+'0';
	}
}
int get_string(char* t)
{
	while ((*t++=*ip++)>=32);
	return t-s;
}
int main()
{
	fread_unlocked(infile,20000000,1,stdin);
//	read(0,infile,20000000);
	ip=infile;
	op=outfile;
	int N,best,cnt,l;
	char *begin,*end;
	int used[256];
	for(;;)
	{
		if (!(N=in())) break;
		getchar();
		l=get_string(s);
		best=cnt=0;
		begin=end=s;
		memset(used,0,sizeof(used));
		while (*end>=32)
		{
			if (++used[*end++]==1)
				cnt++;
			if (cnt>N)
			{
				while (--used[*begin++]);
				cnt=N;
			}
			if (end-begin>best)
				best=end-begin;
			if (l-(begin-s)<=best)
				break;
		}
		printf("%d\n",best);		
//		out(best);
//		*op++='\n';
	}
//	fwrite(outfile,op-outfile,1,stdout);
	return 0;
}
