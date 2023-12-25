// 2009-05-05
#include <iostream>
using namespace std;
int main()
{
	int k;
	for (;;)
	{
		if (scanf("%*d %*d %d",&k)!=1)
			return 0;
		puts(k%2?"no":"yes");
	}
}
