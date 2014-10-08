// 2008-11-25
#include <iostream>
using namespace std;
int main()
{
	int n;
	for(;;)
	{
		scanf("%d",&n);
		if (!n) return 0;
		printf("%d\n",n*(n+1)*(2*n+1)/6);
	}
}
