// 2009-12-21
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	static double prob[600][3600];
	int i,j,k,n;
	prob[0][0]=1;
	for (i=1; i<600; i++)
		for (j=0; j<=6*i; j++)
		{
			prob[i][j]=0.0;
			for (k=j-1; k>=j-6&&k>=0; k--)
				prob[i][j]+=prob[i-1][k]/6.0;
		}
	int T;
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d %d",&n,&k);
		if (n>=600||k>=3600) printf("0\n");
		else printf("%d\n",int(100*prob[n][k]));
	}
	return 0;
}
