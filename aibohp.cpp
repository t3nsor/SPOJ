// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
int main()
{
	int t,L,i,j;
	char s[6101];
	char s2[6101];
	static short a[6101][6101];
	scanf("%d",&t);
	getchar();
	while(t--)
	{
		gets(s);
		L=strlen(s);
		strcpy(s2,s);
		reverse(s2,s2+L);
		for (i=0; i<=L; i++)
		{
			a[i][0]=0;
			a[0][i]=0;
		}
		for (i=1; i<=L; i++)
			for (j=1; j<=L; j++)
				if (s[i-1]==s2[j-1])
					a[i][j]=a[i-1][j-1]+1;
				else
					a[i][j]=max(a[i-1][j],a[i][j-1]);
		printf("%d\n",L-a[L][L]);
	}
	return 0;
}
