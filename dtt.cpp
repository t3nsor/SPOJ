// 2008-06-02
#include <cstdio>
#include <algorithm>
using namespace std;
static char memo[1594323];
int f(int x,int n)
{
	int i,j,k,p1,p2,p3,z,x2;
	if (n<3) return 0;
	if (memo[x]>-1)
		return memo[x];
	memo[x]=0;
	x2=x;
	for (i=1,p1=1; p1<=13; i*=3,p1++) //longest side
	{
		if (x2/i%3==0)
			continue;
		x2-=i;
		for (j=1,p2=1; p2<=p1; j*=3,p2++) //second longest side
		{
			if (x2/j%3==0)
				continue;
			x2-=j;
			for (k=1,p3=1; p3<=p2; k*=3,p3++) //shortest side
			{
				if (p3+p2<=p1)
					continue;
				if (x2/k%3==0)
					continue;
				x2-=k;
				z=f(x2,n-3);
				if (memo[x]<1+z)
					memo[x]=1+z;
				x2+=k;
			}
			x2+=j;
		}
		x2+=i;
	}
	return memo[x];
}
int main()
{
	int N,i;
	int _c1[13],_c2[13];
	int P1,P2,C1,C2;
	memset(memo,-1,sizeof(memo));
	for(;;)
	{
		scanf("%d",&N);
		memset(_c1,0,sizeof(_c1));
		memset(_c2,0,sizeof(_c2));
		P1=0; P2=0; C1=0; C2=0;
		if (N==0) return 0;
		for (i=0; i<(N+1)/2; i++)
		{
			int c;
			scanf("%d",&c); c--;
			_c1[c]++;
			if (_c1[c]==3)
			{
				_c1[c]=0;
				P1++;
			}
			if (i==N/2 && N%2) break;
			scanf("%d",&c); c--;
			_c2[c]++;
			if (_c2[c]==3)
			{
				_c2[c]=0;
				P2++;
			}
		}
		if (P1>P2)
		{
			printf("1\n");
			continue;
		}
		if (P2>P1)
		{
			printf("2\n");
			continue;
		}
		int N1=(N+1)/2-3*P1; int N2=N/2-3*P2;
		int x1=0,x2=0,y=1;
		for (i=0; i<13; i++)
		{
			x1+=_c1[i]*y;
			x2+=_c2[i]*y;
			y*=3;
		}
		C1=f(x1,N1);
		C2=f(x2,N2);
		if (C1>C2)
			printf("1\n");
		else if (C2>C1)
			printf("2\n");
		else
			printf("0\n");
	}
}
