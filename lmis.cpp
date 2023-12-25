// 2009-02-28
#include <iostream>
using namespace std;
int process(char* s,int* p)
{
	int* init=p;
	char c;
	int x;
	for(;;)
	{
		x=0;
		for(;;)
		{
			if ((c=*s++)<=32)
				break;
			x=(x<<1)+(x<<3)+c-'0';
		}
		*p++=x;
		if (!c) return p-init;
	}
}
int main()
{
	int N,i=0,L,l,r,m;
	char input[7000000];
	static int a[1000010];
	static int least[1000010];
	gets(input);
	N=process(input,a);
//	a[N]=2e9;
	//now for the LIS algorithm...
	L=0;
	least[0]=-2e9;
	for (i=0; i<N; i++)
	{
		l=0;
		r=L;
		while (r>l)
		{
			m=(l+r+1)/2;
			if (least[m]<=a[i])
				l=m;
			else
				r=m-1;
		}
		if (l==L)
			least[++L]=a[i];
		else
			least[l+1]=min(least[l+1],a[i]);
	}
	printf("%d\n",N-L);
	return 0;
}
