// 2009-05-25
#include <iostream>
using namespace std;
int main()
{
	static int num[1000001]={0};
	int K,L,T,N,i;
	scanf("%d %d %d",&K,&L,&T);
	num[0]=0; //zero is a P-position
	for (i=1; i<=1000000; i++)
	{
		num[i]=~num[i-1];
		if (i>=K)
			num[i]|=~num[i-K];
		if (i>=L)
			num[i]|=~num[i-L];
	}
	while (T--)
	{
		scanf("%d",&N);
		putchar(num[N]?'A':'B');
	}
	puts("");
	return 0;
}
