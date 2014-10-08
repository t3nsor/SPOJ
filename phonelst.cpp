// 2011-03-20
#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;
bool is_prefix(string& s1,string& s2)
{
	int i;
	for (i=0; i<s1.length(); i++)
		if (i>=s2.length()||s1[i]!=s2[i]) return false;
	return true;
}
int main()
{
	int T,n,i;
	char s[100];
	string a[10000];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&n);
		for (i=0; i<n; i++)
		{
			scanf("%s",s);
			a[i]=s;
		}
		sort(a,a+n);
		bool ok=true;
		for (i=1; i<n; i++)
			if (is_prefix(a[i-1],a[i]))
			{
				ok=false;
				break;
			}
		puts(ok? "YES" : "NO");
	}
	return 0;
}
