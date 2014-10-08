// 2008-09-28
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	char c;
	int base,N,cnt,x,i,num;
	char res[50];
	char s[50];
	for(;;)
	{
		c=getchar();
		if (c=='e') return 0;
		if (c=='t')
		{
			scanf("o%d %d",&base,&N);
			getchar();
			cnt=0;
			while (N)
			{
				res[cnt++]=(N%base-base)%base;
				N=(N-((N%base-base)%base))/base;
			}
			for (i=cnt; i; i--)
				putchar(res[i-1]+'0');
			if (!cnt) putchar('0');
			putchar('\n');
		}
		else
		{
			scanf("rom%d %s",&base,s);
			getchar();
			int x=1;
			num=0;
			for (i=strlen(s)-1; i>=0; i--)
			{
				num+=x*(s[i]-'0');
				x*=base;
			}
			printf("%d\n",num);
		}
	}
}
