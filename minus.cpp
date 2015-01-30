// 2008-08-19
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int T,n,m,i,j,_m,_M;
	int a[100];
	static char dp[100][20000];
	static char last[100][20000];
	char sign[100];
	scanf("%d\n",&T);
	while (T--)
	{
		scanf("%d %d",&n,&m);
		scanf("%d",a);
		if (n==1)
		{
			printf("\n");
			continue;
		}
		memset(dp,0,20000*n);
		scanf("%d",a+1);
		_m=a[0]-a[1]+10000;
		dp[1][_m]=1;
		last[1][_m]='-';
		_M=_m+1;
		for (i=2; i<n; i++)
		{
			scanf("%d",a+i);
			for (j=max(a[i],_m); j<_M; j++)
				if (dp[i-1][j])
				{
					dp[i][j-a[i]]=1;
					last[i][j-a[i]]='-';
				}
			for (j=min(19999-a[i],_M-1); j>=_m; j--)
				if (dp[i-1][j])
				{
					dp[i][j+a[i]]=1;
					last[i][j+a[i]]='+';
				}
			_m=max(0,_m-a[i]);
			_M=min(20000,_M+a[i]);
		}
		m+=10000;
		for (i=n-1; i; i--)
		{
			sign[i]=last[i][m];
			if (last[i][m]-'-')
				m-=a[i];
			else
				m+=a[i];
		}
		i=1;
		int cnt=2;
		while (i<n)
		{
			j=i+1;
			while (j<n&&sign[j]=='+')
			{
				printf("%d\n",cnt);
				j++;
			}
			i=j;
			cnt++;
		}
		while (cnt-->2)
			printf("1\n");
		printf("\n");
	}
	return 0;
}
