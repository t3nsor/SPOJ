// 2008-10-22
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
void inc(char* s)
{
	int cur=0;
	bool ok=false;
	while (!ok)
	{
		s[cur]++;
		if (s[cur]=='K')
			s[cur++]='A';
		else
			ok=true;
	}
	if (s[cur]==1)
	{
		s[cur]='A';
		s[cur+1]=0;
	}
}
int ok(char* s)
{
	int i=1;
	while (s[i])
	{
		if (s[i]==s[i-1]||s[i]==s[i-1]-1||s[i]==s[i-1]+1)
			return 0;
		i++;
	}
	return 1;
}
int hash(char* s)
{
	int x=1;
	int i=0;
	int res=0;
	while (s[i])
	{
		res+=x*(s[i++]-'A'+1);
		x*=11;
	}
	return res;
}
int ans[2000000];
int main()
{
	//precompute answers
	char s1[10]="A",s2[10]="AAAAAAA";
	int sum=0;
	while (strcmp(s1,s2))
	{
		if (ok(s1))
			sum++;
		ans[hash(s1)]=sum;
		inc(s1);
	}
	for(;;)
	{
		s1[0]=0;
		scanf("%s %s",s1,s2);
		if (!s1[0]) return 0;
		reverse(s1,s1+strlen(s1));
		reverse(s2,s2+strlen(s2));
		printf("%d\n",max(ans[hash(s2)]-ans[hash(s1)]-ok(s2),0));
	}
}
