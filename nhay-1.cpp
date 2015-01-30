// 2008-07-14
// nhay-1.cpp uses Rabin-Karp. For KMP, see nhay-2.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
const int RADIX=73939133;
const int RADIX_INVERSE=2257814165;
int f(int x)
{
	int result=1;
	int cur=RADIX;
	while (x)
	{
		if (x&1)
			result*=cur;
		x>>=1;
		cur*=cur;
	}
	return result;
}
int main()
{
	int l,j;
#ifdef _MSC_VER
	freopen("nhay.in","r",stdin);
	freopen("nhay.out","w",stdout);
#endif
	for(;;)
	{
		char c=GC();
		if (c==-1)
			return 0;
		else
			ungetc(c,stdin);
		scanf("%d",&l);
		char* needle=new char[l+10];
		char* hay=new char[l+10];
		int pwr=f(l-1);
		scanf("%s",needle);
		GC();
		int h=needle[l-1];
		for (j=l-2; j>=0; j--)
			h=RADIX*h+needle[j];
		bool ok=true;
		int cur=1;
		int h2=0;
		for (j=0; j<l; j++)
		{
			hay[j]=GC();
			if (hay[j]=='\n')
			{
				ok=false;
				break;
			}
			h2+=cur*hay[j];
			cur*=RADIX;
		}
		if (!ok)
			putchar('\n');
		else
		{
			ok=false;
			int pos=0;
			int first=0;
			for(;;)
			{
				if (h==h2)
				{
					ok=true;
					printf("%d\n",pos);
				}
				c=GC();
				if (c=='\n') break;
				h2=(h2-hay[first])*RADIX_INVERSE+pwr*c;
				hay[first]=c;
				first++;
				if (first==l)
					first=0;
				pos++;
			}
			if (!ok)
				putchar('\n');
		}
		delete needle;
		delete hay;
	}
	return 0;
}
