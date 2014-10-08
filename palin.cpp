// 2008-01-29
#include <stdio.h>
#include <string.h>
int main()
{
	int i,l,t,j,L,U,digit;
	bool less,carry;
	char K[1000001];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%s",&K);
		l=strlen(K);
		less=false;
		L=l/2-1;
		U=(l+1)/2;
		while (L>=0)
		{
			if (K[L]<K[U])
			{
				less=false;
				break;
			}
			if (K[L]>K[U])
			{
				less=true;
				break;
			}
			L--;
			U++;
		}
		if (!less)
		{
			carry=true;
			digit=(l-1)/2;
			while (carry&&(digit>=0))
			{
				K[digit]++;
				if (K[digit]==':')
				{
					K[digit]='0';
					carry=true;
				}
				else
					carry=false;
				digit--;
			}
			if (carry) //10000....00001
			{
				putchar('1');
				for (j=0; j<l-1; j++)
					putchar('0');
				putchar('1');
				putchar('\n');
				continue;
			}
		}
		for (j=0; j<l/2; j++)
			K[l-j-1]=K[j];
		printf("%s\n",K);
	}
	return 0;
}
