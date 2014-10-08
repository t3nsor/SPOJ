// 2007-11-16
#include <iostream>
#define max(x,y) (x>y?x:y)

using namespace std;

static long long a[1000000];

long long f(long long n)
{
	if (n>=1000000)
		return max(n,f(n/2)+f(n/3)+f(n/4));	
	if (a[n]>=0) return a[n];
	long long m;
	if (n<12) m=n;
	else m=max(n,f(n/2)+f(n/3)+f(n/4));
	a[n]=m;
	return m;
}

int main()
{
	int i;
	for (i=0; i<1000000; i++)
		a[i]=-1;
	int N;
	int c;
	c=0;
	while (!cin.eof())
	{
		cin >> N;
		c++;
		cout << f(N) << endl;
		if (c==10) break;
	}
	return 0;
}
