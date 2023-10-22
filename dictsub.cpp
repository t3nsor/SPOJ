// 2023-10-21
#include <stdio.h>
#include <string.h>
char s[10010];
char t[20010];
int st[26][10010];
int next[10010][26];
int main()
{
	int T,N,i,j,k,num;
	int n[26];
	scanf("%d",&T);
	while (T--)
	{
		scanf("%d",&N);
		getchar();
		s[0]='A';
		fgets(s+1, 10009, stdin);
		memset(n,0,sizeof(n));
		for (i=0; s[i] >= 'A'; i++)
		{
			int x=s[i]-'A';
			for (j=0; j<n[x]; j++)
				next[st[x][j]][x]=i;
			n[x]=0;
			for (j=0; j<26; j++)
				st[j][n[j]++]=i;
		}
		for (j=0; j<26; j++)
			for (k=0; k<n[j]; k++)
				next[st[j][k]][j]=i;
		while (N--)
		{
			int pos=0;
			fgets(t, 20010, stdin);
			j=0;
			while (t[j] >= '0')
			{
				if (pos==i) break;
				num=0;
				while (t[j]<='9')
					num=10*num+t[j++]-'0';
				while (num--)
				{
					if (pos==i) break;
					pos=next[pos][t[j]-'A'];
				}
				j++;
			}
			if (pos==i||t[j] >= '0')
				printf("NO\n");
			else 
				printf("YES\n");
		}
		putchar('\n');
	}
	return 0;
}
