// 2008-07-24
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
inline int min(int x,int y)
{
	return x<y?x:y;
}
int main()
{
	int n,i,j,k;
	int color[100][100];
	int smoke[100][100];
	int a[100];
	for(;;)
	{
		if (scanf("%d",&n)==EOF)
			return 0;
		for (i=0; i<n; i++)
		{
			scanf("%d",a+i);
			smoke[i][i]=0;
			color[i][i]=a[i]%100;
		}
		for (i=1; i<n; i++)
			for (j=0; j<n-i; j++)
			{
				smoke[j][i+j]=1000000000;
				for (k=0; k<i; k++)
				{
					color[j][i+j]=(color[j+1][i+j]+a[j])%100;
					smoke[j][i+j]=min(smoke[j][i+j],
						smoke[j][j+k]+smoke[j+k+1][j+i]+color[j][j+k]*color[j+k+1][j+i]);
				}
			}
		printf("%d\n",smoke[0][n-1]);
	}
}
