// 2008-04-21
#include <iostream>
using namespace std;
int main()
{
	int dp[501];
	int b,n,i,cost,fun,j;
	for(;;)
	{
		cin >> b >> n;
		if (b==0 && n==0)
			return 0;
		memset(dp,0,sizeof(dp));
		for (i=0; i<n; i++)
		{
			cin >> cost >> fun;
			for (j=b-cost; j>=0; j--)
				if (dp[j+cost]<dp[j]+fun)
					dp[j+cost]=dp[j]+fun;
		}
		int max=0;
		int c;
		for (i=0; i<=b; i++)
			if (max<dp[i])
			{
				max=dp[i];
				c=i;
			}
		//max = most fun possible
		//c = cost
		if (max==0)
			cout << "0 0\n";
		else
			cout << c << ' ' << max << endl;
	}
	return 0;
}
