// 2008-01-30
#include <stdio.h>
int main()
{
	int a[100001],b[100001];
	int n,i;
	bool B;
	for(;;)
	{
		scanf("%d",&n);
		if (n==0) return 0;
		for (i=1; i<=n; i++)
		{
			scanf("%d",&a[i]);
			b[a[i]]=i;
		}
		B=true;
		for (i=1; i<=n; i++)
			if (a[i]!=b[i])
			{
				B=false;
				break;
			}
		if (B)
			printf("ambiguous\n");
		else
			printf("not ambiguous\n");
	}
}
