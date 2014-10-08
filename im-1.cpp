// 2009-12-24
// Another example of how to use Lengauer-Tarjan as a sledgehammer
#include <iostream>
#include <vector>
#include <algorithm>
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
	bool neg=false;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		if (c=='-') neg=true;
		else x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return neg?-x:x;
}
int main()
{
	int T,V,E,p,q;
	T=in();
	while (T--)
	{
		V=in(); E=in();
		succ=vector<vector<int> >(V+1);
		while (E--)
		{
			p=in(); q=in();
			if (p<=0||p>V||q<=0||q>V) continue;
			succ[q].push_back(p);
			succ[p].push_back(q);
		}
		dominators(2);
		vector<int> V1,V2,V3;
		if (!dom[1]||!dom[3])
		{
			printf("NO\n");
			continue;
		}
		int i;
		i=1;
		while (i!=2)
		{
			V1.push_back(i);
			i=dom[i];
		}
		i=3;
		while (i!=2)
		{
			V2.push_back(i);
			i=dom[i];
		}
		sort(V1.begin(),V1.end());
		sort(V2.begin(),V2.end());
		V3.resize(V1.size()+V2.size());
		if (set_intersection(V1.begin(),V1.end(),V2.begin(),V2.end(),
		  V3.begin())==V3.begin())
			printf("YES\n");
		else
			printf("NO\n");
	}
	return 0;
}
