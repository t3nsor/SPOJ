// 2008-06-21
#include <cstdio>
#include <cmath>
unsigned int primes[7000];
bool prime[65536];
void sieve()
{
	int i,j,k=0;
	prime[0]=false;
	prime[1]=false;
	for (i=2; i<=65535; i++)
		prime[i]=true;
	for (i=2;;i++)
	{
		while ((i<=65535)&&(!prime[i])) i++;
		if (i>65535) break;
		primes[k++]=i;
		for (j=2*i; j<=65535; j+=i)
			prime[j]=false;
	}
}
bool IsPrime(unsigned int x)
{
	int i;
	for (i=0; i<6542 && primes[i]*primes[i]<=x; i++)
		if (x%primes[i]==0)
			return false;
	return true;
}
unsigned int FindPrime(unsigned int x)
{
	while (!IsPrime(x))
	{
		if (x%6==1) x-=2;
		else if (x%6==5) x-=4;
		else x--;
	}
	return x;
}
int main()
{
	sieve();
	int T,i,N,K,j;
	unsigned int a[200];
	unsigned int b[200];
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		if (i==57)
		{
			i++;
			i--;
		}
		scanf("%d %d",&N,&K);
		for (j=0; j<N; j++)
		{
			scanf("%u",a+j);
			b[j]=FindPrime(a[j]);
		}
		unsigned int result=0;
		if (N==1)
		{
			if (a[0]==b[0])
				b[0]=FindPrime(a[0]-1);
			if ((a[0]-b[0])%(K+1))
				result=1;
			else
				result=0;
		}
		else
			for (j=0; j<N; j++)
			{
				if (a[j]-b[j]<=K)
				{
					result=1;
					break;
				}
				result^=(a[j]-b[j])%(K+1);
			}
		if (result==0)
			printf("NO\n");
		else
			printf("YES\n");
	}
	return 0;
}
