// 2010-05-05
// TODO: Is it actually necessary to use the Lengauer-Tarjan algorithm for
// this problem?
#include <iostream>
#include <iterator>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;
// Lengauer-Tarjan algorithm begins here
vector<vector<int> > succ; //1..n
vector<int> dom; //1..n
vector<int> parent,ancestor,child,vertex; //1..n
vector<int> label,semi,size; //0..n
vector<vector<int> > pred,bucket; //1..n
int n;
void dfs(int v){
	semi[v]=++n;
	vertex[n]=label[v]=v;
	ancestor[v]=child[v]=0;
	size[v]=1;
	for (int i=0; i<succ[v].size(); i++)
	{
		int w=succ[v][i];
		if (!semi[w])
		{
			parent[w]=v;
			dfs(w);
		}
		pred[w].push_back(v);
	}
}
void compress(int v){
	if (ancestor[ancestor[v]])
	{
		compress(ancestor[v]);
		if (semi[label[ancestor[v]]]<semi[label[v]])
			label[v]=label[ancestor[v]];
		ancestor[v]=ancestor[ancestor[v]];
	}
}
/*
int eval(int v){
	if (!ancestor[v])
		return v;
	else
	{
		compress(v);
		return label[v];
	}
}
void link(int v,int w){
	ancestor[w]=v;
}
*/
int eval(int v){
	if (!ancestor[v])
		return label[v];
	else
	{
		compress(v);
		return semi[label[ancestor[v]]]>=semi[label[v]]?
		  label[v]:label[ancestor[v]];
	}
}
void link(int v,int w){
	int s=w;
	while (semi[label[w]]<semi[label[child[s]]])
		if (size[s]+size[child[child[s]]]>=2*size[child[s]])
		{
			ancestor[child[s]]=s;
			child[s]=child[child[s]];
		}
		else
		{
			size[child[s]]=size[s];
			s=ancestor[s]=child[s];
		}
	label[s]=label[w];
	size[v]+=size[w];
	if (size[v]<2*size[w])
		swap(s,child[v]);
	while (s)
	{
		ancestor[s]=v;
		s=child[s];
	}
}
void dominators(int r){
	int u,v,w;
	n=succ.size()-1;
	dom=parent=ancestor=child=vertex=label=semi=size=
	  vector<int>(n+1,0);
	pred=bucket=vector<vector<int> >(n+1);
	n=0;
	dfs(r);
	int i,j;
	for (i=n; i>=2; i--)
	{
		int w=vertex[i];
		for (j=0; j<pred[w].size(); j++)
		{
			v=pred[w][j];
			u=eval(v);
			if (semi[u]<semi[w])
				semi[w]=semi[u];
		}
		bucket[vertex[semi[w]]].push_back(w);
		link(parent[w],w);
		for (j=0; j<bucket[parent[w]].size(); j++)
		{
			v=bucket[parent[w]][j];
			u=eval(v);
			dom[v]=semi[u]<semi[v]?u:parent[w];
		}
	}
	for (i=2; i<=n; i++)
	{
		w=vertex[i];
		if (dom[w]!=vertex[semi[w]])
			dom[w]=dom[dom[w]];
	}
	dom[r]=0;
}
// Lengauer-Tarjan algorithm ends here
int in()
{
	char c=0;
	int x=0;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int main()
{
	int T,V,E,p,q;
	T=10;
	while (T--)
	{
		V=in(); E=in();
		succ=vector<vector<int> >(V+1);
		while (E--)
		{
			p=in(); q=in();
			succ[p].push_back(q);
		}
		dominators(1);
		vector<int> lol;
		int i;
		for (i=2; i<=V; i++)
			lol.push_back(dom[i]);
		sort(lol.begin(),lol.end());
		vector<int>::iterator It=unique(lol.begin(),lol.end());
		printf("%d\n",It-lol.begin());
		for (vector<int>::iterator It2=lol.begin(); It2!=It; It2++)
			printf("%d ",*It2);
		printf("\n");
	}
	return 0;
}
