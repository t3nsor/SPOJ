// 2009-05-05
#include <iostream>
#include <cstring>
int main()
{
	int t;
	char s[100000];
	scanf("%d",&t);
	while (t--)
	{
		scanf("%*d %s",s);
		int N=strlen(s);
		puts(s+N-1);
	}
	return 0;
}
