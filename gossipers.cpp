// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <map>
using namespace std;
int main()
{
	int i,j,N,M;
	char s[100],s1[100],s2[100];
	string S;
	static int a[2100][2100];
	for(;;)
	{
		scanf("%d %d",&N,&M);
		if (!N) return 0;
		map<string,int> _M;
		for (i=0; i<N; i++)
		{
			scanf("%s",s);
			S=s;
			_M[s]=i;
			memset(a[i],false,N);
			a[i][i]=true;
		}
		for (i=0; i<M; i++)
		{
			scanf("%s %s",&s1,&s2);
			int p1=_M[s1];
			int p2=_M[s2];
			for (j=0; j<N; j++)
				a[p1][j]=a[p2][j]|=a[p1][j];
		}
		bool b=true;
		for (i=0; i<N&&b; i++)
			for (j=0; j<N&&b; j++)
				if (!a[i][j])
					b=false;
		if (b)
			printf("YES\n");
		else
			printf("NO\n");
	}
}
