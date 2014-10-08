// 2008-12-30
#include <cstdio>
#include <cstring>
int main()
{
	int L,i,j,n;
	char s[200010];
	gets(s);
	L=strlen(s);
	memcpy(s+L,s,L);
	i=0;
	j=1;
	n=0;
	while (i<L&&j<L&&n<L)
	{
		if (s[i+n]==s[j+n])
			n++;
		else
		{
			if (s[i+n]<s[j+n])
				j+=n+1;
			else
				i+=n+1;
			if (i==j)
				j=i+1;
			n=0;
		}
	}
	printf("%d\n",(i<j?i:j));
	return 0;
}
