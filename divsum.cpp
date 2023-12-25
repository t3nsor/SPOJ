// 2008-01-29
#include <stdio.h>
#include <math.h>
bool prime[500001];
int factor[500001];
void sieve()
{
	int i,j;
	for (i=2; i<=500000; i++)
	{
		prime[i]=true;
		factor[i]=i;
	}
	for (i=2;;i++)
	{
		while ((!prime[i])&&(i<=701)) i++;
		if (i>701) break;
		for (j=2*i; j<=500000; j+=i)
		{
			prime[j]=false;
			factor[j]=i;
		}
	}
}
int main()
{
	sieve();
	int N,i,n,n2,k,divsum,sum;
	scanf("%d",&N);
	for (i=0; i<N; i++)
	{
		scanf("%d",&n);
		n2=n;
		if (n==1)
			printf("0\n");
		else
		{
			divsum=1;
			while (n>1)
			{
				k=factor[n];
				sum=1;
				while (n%k==0)
				{
					n/=k;
					sum=k*sum+1;
				}
				divsum*=sum;
			}
			printf("%d\n",divsum-n2);
		}
	}
	return 0;
}
