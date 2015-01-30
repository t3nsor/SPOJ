// 2008-08-28
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#ifdef ONLINE_JUDGE
#define pc putchar_unlocked
#else
#define pc putchar
#endif
using namespace std;
int mx;
const int id[]={0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,3};
struct TrieNode
{
	int count;
	TrieNode* link[4];
};
TrieNode memory[2000000];
int top=1;
TrieNode* root;
short a[20001];
int M;
void insert(char* s)
{
	TrieNode* cur=root;
	int i;
	char c;
	for (i=0; i<M; i++)
	{
		c=id[s[i]-'A'];
		if (cur->link[c])
			cur=cur->link[c];
		else
		{
			cur->link[c]=memory+top++;
			cur=cur->link[c];
			memset(cur,0,20);
		}
	}
	a[cur->count++]--;
	if (cur->count>mx)
		a[++mx]=1;
	else
		a[cur->count]++;
}
void output(int x)
{
	int z=1;
	while ((z<<1)+(z<<3)<=x)
		z=(z<<1)+(z<<3);
	while (z)
	{
		pc(x/z%10+'0');
		z/=10;
	}
	pc('\n');
}
char s[500000];
int main()
{
	int N,i,j;
	for(;;)
	{
		scanf("%d %d",&N,&M);
		if (!N)
			return 0;
		root=memory+top++;
		memset(root,0,20);
		mx=0;
		a[0]=N;
		fread(s,(M+1)*N,1,stdin);
		for (i=0; i<N; i++)
			insert(s+(M+1)*i+1);
		for (i=1; i<=mx; i++)
			output(a[i]);
		for (i=mx+1; i<=N; i++)
		{
			pc('0');
			pc('\n');
		}
	}
}
