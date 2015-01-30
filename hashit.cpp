// 2008-07-14
#define CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
char hashtable[101][16];
int hash(char* s)
{
	int h=0,i;
	for (i=0; s[i]; i++)
		h+=s[i]*(i+1);
	return (19*h)%101;
}
void add(char* s)
{
	int i;
	for (i=0; i<101; i++)
		if (!strcmp(s,hashtable[i]))
			return;
	int h=hash(s);
	for (i=0; i<20; i++)
		if (!hashtable[(h+i*i+23*i)%101][0])
		{
			strcpy(hashtable[(h+i*i+23*i)%101],s);
			return;
		}
}
void del(char* s)
{
	int i;
	for (i=0; i<101; i++)
		if (!strcmp(s,hashtable[i]))
		{
			hashtable[i][0]=0;
			return;
		}
}
int main()
{
	int t,i,j,n;
	char st[20];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		for (j=0; j<101; j++)
			hashtable[j][0]=0;
		scanf("%d",&n);
		for (j=0; j<n; j++)
		{
			scanf("%s",st);
			if (st[0]=='A')
				add(st+4);
			else
				del(st+4);
		}
		int c=0;
		for (j=0; j<101; j++)
			if (hashtable[j][0]) c++;
		printf("%d\n",c);
		for (j=0; j<101; j++)
			if (hashtable[j][0])
				printf("%d:%s\n",j,hashtable[j]);
	}
	return 0;
}
