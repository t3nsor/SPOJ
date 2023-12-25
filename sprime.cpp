// 2009-04-06
#include <math.h>
#include <stdio.h>
int primes[1000000];
bool prime[11111111];
int sieve()
{
	int i,j,k=0;
	prime[0]=false;
	prime[1]=false;
	for (i=2; i<11111111; i++)
		prime[i]=true;
	for (i=2;;i++)
	{
		while ((i<11111111)&&(!prime[i])) i++;
		if (i>=11111111) break;
		primes[k++]=i;
		for (j=2*i; j<11111111; j+=i)
			prime[j]=false;
	}
	return k;
}
int main()
{
	int N=sieve();
	int i;
	for (i=0; i<=N&&primes[i]<10000000; i++)
		if (prime[i+1])
			printf("%d\n",primes[i]);
	return 0;
}
