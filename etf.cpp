// 2009-08-31
#include <iostream>
using namespace std;
int primes[100];
int cnt=0;
bool prime(int x)
{
	int i;
	for (i=2; i*i<=x; i++)
		if (x%i==0)
			return false;
	return true;
}
void precompute()
{
	int i;
	for (i=2; i<=1000; i++)
		if (prime(i))
			primes[cnt++]=i;
}
int tot(int x)
{
	int res=1,i;
	if (x==1) return 1;
	for (i=0; i<cnt; i++)
		if (x%primes[i]==0)
		{
			res=primes[i]-1;
			x/=primes[i];
			while (x%primes[i]==0)
			{
				res*=primes[i];
				x/=primes[i];
			}
			return res*tot(x);
		}
	return x-1;
}
int main()
{
	precompute();
	int T,x;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&x);
		printf("%d\n",tot(x));
	}
	return 0;
}
