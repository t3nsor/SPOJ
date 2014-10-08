// 2010-08-09
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int in()
{
    char c;
    do
	c=getchar_unlocked();
    while (c<=32);
    int x=0;
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
    for (;;)
    {
	int N,M,x,y,z,a,b,c,i,j;
	vector<pair<int,pair<int,int> > > V;
	N=in(); M=in(); if (!N) return 0;
	x=in(); y=in(); z=in();
	if (y>x) swap(x,y);
	if (z>y) swap(y,z);
	if (y>x) swap(x,y);
	for (i=0; i<M; i++)
	{
	    a=in(); b=in(); c=in();
	    if (b>a) swap(a,b);
	    if (c>b) swap(b,c);
	    if (b>a) swap(a,b);
	    V.push_back(make_pair(a,make_pair(b,c)));
	}
	sort(V.begin(),V.end());
	int best=1e9;
	for (i=0; i<V.size(); )
	{
	    j=i;
	    while (j<V.size()&&V[j]==V[i]) j++;
	    if (V[i].first>=x&&V[i].second.first>=y&&V[i].second.second>=z&&
		(j-i)>=N)
		best<?=(V[i].first*V[i].second.first*V[i].second.second)-x*y*z;
	    i=j;
	}
	if (best==1e9)
	    printf("impossible\n");
	else
	    printf("%d\n",best);
    }
}
