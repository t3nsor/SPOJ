// 2008-07-14
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int adj[26][26];
int beg[26];
int end[26];
bool reach[26];
void dfs(int x)
{
	reach[x]=true;
	int i;
	for (i=0; i<26; i++)
		if (adj[x][i]&&!reach[i])
			dfs(i);
}
bool conn(int x)
{
	int i;
	for (i=0; i<26; i++)
		reach[i]=false;
	dfs(x);
	for (i=0; i<26; i++)
		if (beg[i]&&!reach[i])
			return false;
	return true;
}
int main()
{
	int t,i,n,j,k;
	char s[1050];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d",&n);
		for (j=0; j<26; j++)
		{
			beg[j]=0;
			end[j]=0;
			for (k=0; k<26; k++)
				adj[j][k]=0;
		}
		for (j=0; j<n; j++)
		{
			scanf("%s",&s);
			char b=s[0];
			k=0; while (s[k]) k++; k--; char e=s[k];
			beg[b-'a']++;
			end[e-'a']++;
			adj[b-'a'][e-'a']++;
		}
		int _count=0;
		for (j=0; j<26; j++)
			if (beg[j]!=end[j])
				if (beg[j]==end[j]+1||beg[j]+1==end[j])
					_count++;
				else
				{
					_count=100;
					break;
				}
		if (_count>2)
		{
			printf("The door cannot be opened.\n");
			continue;
		}
		bool b=false;
		for (j=0; j<26; j++)
			if (beg[j]&&conn(j))
			{
				b=true;
				break;
			}
		if (b)
			printf("Ordering is possible.\n");
		else
			printf("The door cannot be opened.\n");
	}
	return 0;
}
