// 2008-07-15
#include <cstdio>
int main()
{
	char s[30];
	bool vis[26];
	int len[27];
	int i,j,k,/*m,*/t;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%s",&s);
		for (j=0; j<26; j++)
			vis[j]=false;
		for (j=0; j<=26; j++)
			len[j]=0;
		for (j=0; j<26; j++)
		{
			if (vis[j]) continue;
			k=j;
			int c=0;
			while (!vis[k])
			{
				vis[k]=true;
				k=s[k]-'A';
				//for (m=0; s[m]!='A'+k; m++);
				//k=m;
				c++;
			}
			len[c]++;
		}
		bool b=true;
		for (j=2; j<=26; j+=2)
			if (len[j]&1)
			{
				b=false;
				break;
			}
		if (b)
			printf("Yes\n");
		else
			printf("No\n");
	}
	return 0;
}
