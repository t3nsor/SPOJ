// 2008-01-29
#include <math.h>
#include <stdio.h>
int primes[3401];
bool prime[31608];
void sieve()
{
	int i,j,k=0;
	prime[0]=false;
	prime[1]=false;
	for (i=2; i<=31607; i++)
		prime[i]=true;
	for (i=2;;i++)
	{
		while ((i<=31607)&&(!prime[i])) i++;
		if (i>31607) break;
		primes[k++]=i;
		for (j=2*i; j<=31607; j+=i)
			prime[j]=false;
	}
}
int main()
{
	int t,i,m,n,j,k;
	bool prime2[100001];
	sieve();
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		if (i>0) putchar('\n');
		scanf("%d %d",&m,&n);
		int s=(int)sqrt(n);
		int p;
		for (j=0; j<=100000; j++)
			prime2[j]=true;
		for (j=0;(j<3401)&&(primes[j]<=s);j++)
		{
			p=primes[j];
			for (k=p*(m/p)+p-m;k<=n-m;k+=p)
				prime2[k]=false;
			if (m%p==0) prime2[0]=false;
		}
		for (j=0; j<=n-m; j++)
		{
			if (j+m>31607)
				if (prime2[j])
					printf("%d\n",j+m);
			if (j+m<=31607)
				if (prime[j+m])
					printf("%d\n",j+m);
		}
	}
	return 0;
}
