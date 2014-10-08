// 2008-06-10
#include <cstdio>
#ifndef _MSC_VER
#define GC getchar_unlocked
#else
#define GC getchar
#endif
inline void swap(char& c1,char& c2)
{
	char c=c1;
	c1=c2;
	c2=c;
}
void input(char* s,int& L)
{
	char c;
	L=0;
	for(;;)
	{
		c=GC();
		if (c==-1||c=='\n')
			return;
		s[L++]=c-'0';
	}
}
void div5(char* x,int& L)
{
	int i,c=0,C;
	for (i=L-1; i>=0; i--)
	{
		C=x[i]+10*c;
		x[i]=C/5;
		c=C%5;
	}
	if (x[L-1]==0)
		L--;
}
void convert(char* ten,int L,char* five,int& M)
{
	M=0;
	while (L>0)
	{
		five[M++]=ten[0]%5;
		div5(ten,L);
	}
}
int main()
{
	char lookup[101]="0626422428448466626488682028842466848226628848644204244284884686664244826220882426648422866882484462";
	char base10[100];
	char base5[144];
	int L1,L2,i,x;
	for (i=0; i<100; i++)
		lookup[i]-='0';
	for(;;)
	{
		input(base10,L1);
		if (L1==0)
			return 0;
		if (L1==1&&base10[0]==1)
		{
			printf("1\n");
			continue;
		}
		for (i=0; i<L1/2; i++)
			swap(base10[i],base10[L1-i-1]);
		convert(base10,L1,base5,L2);
		x=0;
		for (i=L2-1; i>=0; i--)
			x=lookup[25*(i%4)+5*x/2+base5[i]];
		putchar(x+'0');
		putchar('\n');
	}
}
