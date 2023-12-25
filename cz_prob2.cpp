// 2008-06-28
#include <cstdio>
#include <cmath>
#ifdef _MSC_VER
#define ll __int64
#define fs "%I64d\n"
#else
#define ll long long
#define fs "%lld\n"
#endif
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
ll factor(ll x)
{
	if (x<31608&&prime[x])
		return x;
	ll sqrtx=sqrt(x);
	for (int i=0; i<3401&&primes[i]<=sqrtx; i++)
		if (x%primes[i]==0)
			return primes[i];
	return x;
}
ll sod(ll x)
{
	ll ans=1;
	while (x>1)
	{
		ll f=factor(x);
		ll c=1;
		while (x%f==(ll)0)
		{
			x/=f;
			c=c*f+1;
		}
		ans*=c;
	}
	return ans;
}
int main()
{
	int T,i,N;
	sieve();
	scanf("%d",&T);
	for (i=0; i<T; i++)
	{
		scanf("%d",&N);
		printf(fs,sod(N));
	}
	return 0;
}
