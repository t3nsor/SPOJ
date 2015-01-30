// 2008-06-21
// modified 2014-10-07
#include <cstdio>
typedef long long i64;
int main()
{
	int i,K,N;
	scanf("%d",&K);
	for (i=0; i<K; i++)
	{
		scanf("%d",&N);
		int N2=(N-1)*(N-1);
		i64 prod=1;
		int p2=1;
		while (p2<=N2)
			p2<<=1;
		p2>>=1;
		while (p2)
		{
			prod=(prod*prod)%(i64)98777;
			if (N2&p2)
			{
				prod<<=1;
				if (prod>=98777)
					prod-=98777;
			}
			p2>>=1;
		}
		printf("%d\n",int(prod));
	}
	return 0;
}
