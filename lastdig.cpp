// 2009-03-18
#include <iostream>
using namespace std;
int main()
{
	int t,a,b,c;
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d %d",&a,&b);
		if (b>4) b=(b%4)+4;
		int c=1;
		while (b--) c=(a*c)%10;
		printf("%d\n",c);
	}
	return 0;
}
