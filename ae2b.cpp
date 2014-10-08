// 2009-12-21
#include <iostream>
using namespace std;
int gcd(int x,int y)
{
	if (x==0) return y;
	else return gcd(y%x,x);
}
int ABS(int x)
{
	return x>0?x:-x;
}
char* ipos;
char infile[20000000];
int in()
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
int main()
{
	ipos=infile;
	fread_unlocked(infile,20000000,1,stdin);
	int T;
	T=in();
	while (T--)
	{
		int K,N,x1,y1,x2,y2,dx,dy;
		K=in(); N=in(); x1=in(); y1=in(); x2=in(); y2=in();
		K=ABS(K);
		N=ABS(N);
		dx=ABS(x1-x2);
		dy=ABS(y1-y2);
		int f=gcd(K,N);
		if (dx%f||dy%f)
			printf("NIE\n");
		else
		{
			K/=f; N/=f; dx/=f; dy/=f;
			if (K%2 && N%2) //parity?
				if (ABS(dx-dy)%2)
					printf("NIE\n");
				else
					printf("TAK\n");
			else
				printf("TAK\n");
		}
	}
	return 0;
}
