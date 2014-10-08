// 2008-02-02
// modified 2014-10-07
#include <stdio.h>
#include <string.h>
typedef long long int64;
int main()
{
	int64 a[5001];
	int i;
	char s[5005];
	for(;;)
	{
		scanf("%s",s);
		int l=strlen(s);
		if ((l==1)&&(s[0]=='0'))
			return 0;
		for (i=0; i<=l; i++)
			a[i]=0;
		a[0]=1;
		for (i=0; i<l; i++)
		{
			if (s[i]=='0') continue;
			a[i+1]+=a[i];
			if (i==l-1) continue;
			if (s[i]>'2') continue;
			if ((s[i]=='2')&&(s[i+1]>'6')) continue;
			a[i+2]+=a[i];
		}
		printf("%lld\n", a[l]);
	}
}
