// 2008-08-02
// modified 2014-10-07
const int items = 15000000;
#include <cstdio>
int main()
{
	int T,i,s,t,N,M,begin,sum;
	static int rain[items];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d %d %d %d",&s,&t,&N,&M);
		sum=0;
		begin=0;
		int m=1000000000;
		for (i=0; i<N; i++)
		{
			s=(78901+31*s)%699037;
			t=(23456+64*t)%2097151;
			rain[i]=(s%100+1)*(t%100+1);
			sum+=rain[i];
			while (sum>M&&begin<=i)
			{
				sum-=rain[begin];
				begin++;
			}
			if (begin>0&&m>i-begin+1)
				m=i-begin+1;
		}
		if (m==1000000000)
			m=N;
		printf("%d\n",m);
	}
	return 0;
}
