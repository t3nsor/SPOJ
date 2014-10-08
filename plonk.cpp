// 2010-12-11
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int in()
{
    char c;
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
template<class RAI>
void radixPass(RAI a,RAI b,int n,int place)
{
	static int cnt[512];
	memset(cnt,0,sizeof(cnt));
	static int cum[512];
	int i;
	for (i=0; i<n; i++)
		cnt[(a[i].first&(0x1ff<<(9*place)))>>(9*place)]++;
	for (i=0; i<512; i++)
		cum[i]=(i==0)?0:cum[i-1]+cnt[i-1];
	for (i=0; i<n; i++)
		b[cum[(a[i].first&(0x1ff<<(9*place)))>>(9*place)]++]=a[i];
}
int main()
{
    int T,N,i,d,X,Y;
    T=in();
    pair<int,int> a[10000],b[10000],tmp[10000];
    int dist[10000];
    while (T--)
    {
	N=in();
	for (i=0; i<N; ++i)
	{
	    X=in(); Y=in();
	    a[i]=make_pair(X,i);
	    b[i]=make_pair(Y,i);
	}
	radixPass(a,tmp,N,0);
	radixPass(tmp,a,N,1);
	radixPass(b,tmp,N,0);
	radixPass(tmp,b,N,1);
	d=0;
	for (i=1; i<N; ++i)
	{
	    d+=(a[i].first-a[i-1].first)*i;
	    dist[a[i].second]=d;
	}
	dist[a[0].second]=0;
	d=0;
	for (i=N-2; i>=0; --i)
	{
	    d+=(a[i+1].first-a[i].first)*(N-i-1);
	    dist[a[i].second]+=d;
	}
	d=0;
	for (i=1; i<N; ++i)
	{
	    d+=(b[i].first-b[i-1].first)*i;
	    dist[b[i].second]+=d;
	}
	d=0;
	for (i=N-2; i>=0; --i)
	{
	    d+=(b[i+1].first-b[i].first)*(N-i-1);
	    dist[b[i].second]+=d;
	}
	int res=2e9;
	for (i=0; i<N; i++)
	    res=min(res,dist[i]);
	printf("%d\n",res);
    }
    return 0;
}
