// 2008-07-29
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <list>
using namespace std;
int main()
{
	int N,i,u,v;
	scanf("%d",&N);
	list<int> L[10000];
	for (i=0; i<N-1; i++)
	{
		scanf("%d %d",&u,&v); u--,v--;
		L[u].push_back(v);
		L[v].push_back(u);
	}
	int size=1;
	int p[20000];
	int n[20000];
	int d[20000];
	int maxdist=0;
	int node;
	p[0]=-1;
	n[0]=0;
	d[0]=0;
	while (size)
	{
		size--;
		int _n=n[size];
		int _d=d[size];
		int _p=p[size];
		if (maxdist<_d)
		{
			maxdist=_d;
			node=_n;
		}
		list<int>::iterator I;
		for (I=L[_n].begin();I!=L[_n].end();I++)
			if (*I!=_p)
			{
				p[size]=_n;
				n[size]=*I;
				d[size]=_d+1;
				size++;
			}
	}
	size=1;
	maxdist=0;
	p[0]=-1;
	n[0]=node;
	d[0]=0;
	while (size)
	{
		size--;
		int _n=n[size];
		int _d=d[size];
		int _p=p[size];
		if (maxdist<_d)
		{
			maxdist=_d;
			node=_n;
		}
		list<int>::iterator I;
		for (I=L[_n].begin();I!=L[_n].end();I++)
			if (*I!=_p)
			{
				p[size]=_n;
				n[size]=*I;
				d[size]=_d+1;
				size++;
			}
	}
	printf("%d\n",maxdist);
	return 0;
}
