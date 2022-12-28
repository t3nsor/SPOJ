// 2009-05-05
#include <iostream>
using namespace std;
struct trienode
{
	trienode* next[26];
	trienode()
	{
		memset(next,0,sizeof(next));
	}
	/*~trienode()
	{
		for (int i=0; i<26; i++)
			if (next[i])
				delete next[i];
	}*/
};
struct trie
{
	trienode* root;
	int size;
	trie()
	{
		root=new trienode;
		size=0;
	}
/*	~trie()
	{
		delete root;
	}*/
	void insert(char* s)
	{
		trienode *p=root, *q;
		int i;
		for (i=0; s[i]; i++)
		{
			if (!p->next[s[i]-'A']) //doesn't exist
			{
				size++;
				p->next[s[i]-'A']=new trienode;
			}
			p=p->next[s[i]-'A'];
		}
	}
};
int main()
{
	int T,i;
	char s[1010];
	scanf("%d",&T);
	trie* t;
	while (T--)
	{
		t=new trie;
		scanf("%s",s);
		for (i=0; s[i]; i++)
			t->insert(s+i);
		printf("%d\n",t->size);
	}
	//insert all non-empty suffixes
	return 0;
}
