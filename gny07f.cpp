// 2008-08-20
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
int main()
{
	int i,j,T,depth,m;
	char s[200];
	scanf("%d",&T);
	getchar();
	for (i=1; i<=T; i++)
	{
		depth=m=0;
		gets(s);
		for (j=0; s[j]; j++)
		{
			if (s[j]-'[')
				depth--;
			else
			{
				depth++;
				if (depth>m)
					m=depth;
			}
		}
		printf("%d %d\n",i,1<<m);
	}
	return 0;
}
