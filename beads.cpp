// 2008-07-20
// Note: I'm pretty sure I cheated on this problem by looking at the solution
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
int main()
{
	int T,L,i,j,n;
	scanf("%d\n",&T);
	char s[10010];
	while (T--)
	{
		gets(s);
		L=strlen(s);
		i=0;
		j=1;
		n=0;
		while (i<L&&j<L&&n<L)
		{
			if (s[(i+n)%L]==s[(j+n)%L])
				n++;
			else
			{
				if (s[(i+n)%L]<s[(j+n)%L])
					j+=n+1;
				else
					i+=n+1;
				if (i==j)
					j=i+1;
				n=0;
			}
		}
		printf("%d\n",(i<j?i:j)+1);
	}
	return 0;
}
