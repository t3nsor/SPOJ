// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cassert>
#include <string>
#include <queue>
#include <set>
#include <map>
#define MAX_PACKAGES 20000
using namespace std;
map<string,int> M;
int packages=0;
int getid(string S)
{
	int res;
	map<string,int>::iterator It=M.find(S);
	if (It==M.end())
		M.insert(pair<string,int>(S,res=packages++));
	else
		res=It->second;
	assert (packages<MAX_PACKAGES);
	return res;
}
bool exist[MAX_PACKAGES];
set<int> this_depends_on[MAX_PACKAGES];
set<int> depend_on_this[MAX_PACKAGES];
int this_depends_on_howmany[MAX_PACKAGES];
int main()
{
	int i,id,id2;
	char s[10000];
	char s2[10000];
#ifndef ONLINE_JUDGE
	freopen("depend.in","r",stdin);
#endif
	for(;;)
	{
		s[0]=0;
		scanf("%s",s);
		if (!s[0])
			break;
		id=getid(string(s));
		exist[id]=true;
		for(;;)
		{
			scanf("%s",s2);
			if (s2[0]=='0'&&!s2[1])
				break;
			id2=getid(string(s2));
			this_depends_on[id].insert(id2);
			depend_on_this[id2].insert(id);
		}
	}
	queue<int> Q;
	for (i=0; i<packages; i++)
	{
		this_depends_on_howmany[i]=this_depends_on[i].size();
		if (exist[i]&&!this_depends_on_howmany[i])
			Q.push(i);
	}
	int res=0;
	while (!Q.empty())
	{
		int id=Q.front();
		Q.pop();
		res++;
		set<int>::iterator It;
		for (It=depend_on_this[id].begin(); It!=depend_on_this[id].end(); It++)
			if ((this_depends_on_howmany[*It]--)==1)
				Q.push(*It);
	}
	printf("%d\n",res);
	return 0;
}
