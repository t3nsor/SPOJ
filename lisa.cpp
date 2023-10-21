// 2023-10-21
// originally written 2008-09-30 but with compilation errors
#include <algorithm>
#include <stdio.h>
#include <string.h>
using namespace std;
int main()
{
	int T,i,N,j,k;
	unsigned long long m[100][100];
	unsigned long long M[100][100];
	scanf("%d",&T);
	char s[100];
	getchar();
	while (T--)
	{
		fgets(s, 100, stdin);
		N=(strlen(s)-1)/2+1;
		for (i=0; i<N; i++)
			M[i][i]=m[i][i]=s[i<<1]-'0';
		for (j=1; j<N; j++)
			for (i=0; j+i<N; i++)
			{
				M[i][i+j]=0;
				m[i][i+j]=0xFFFFFFFFFFFFFFFF;
				for (k=i; k<i+j; k++)
				{
					M[i][i+j]=max(M[i][i+j],(s[(k<<1)+1]-'+')?M[i][k]*M[k+1][i+j]:M[i][k]+M[k+1][i+j]);
					m[i][i+j]=min(m[i][i+j],(s[(k<<1)+1]-'+')?m[i][k]*m[k+1][i+j]:m[i][k]+m[k+1][i+j]);
				}
			}
		printf("%llu %llu\n",M[0][N-1],m[0][N-1]);
	}
}
