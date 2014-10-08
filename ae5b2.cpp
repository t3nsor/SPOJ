// 2009-12-30
//Dynamic max prefix sum query with segment tree. O((n+m) lg n)
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
static int sum[1000000];
static int max_psum[1000000];
void recalc(int root)
{
	sum[root]=sum[root<<1]+sum[1+(root<<1)];
	max_psum[root]=max(max_psum[root<<1],
	sum[root<<1]+max_psum[1+(root<<1)]);
}
void atomic_upd(int root,int val)
{
	sum[root]=val;
	max_psum[root]=val>0?val:0;
}
template<class RAI>
void upd(int root,RAI begin,RAI end,RAI pos,int val)
{
	if (end-begin==1)
		atomic_upd(root,val);
	else
	{
		RAI mid=begin+(end-begin)/2;
		if (pos<mid)
			upd(root<<1,begin,mid,pos,val);
		else
			upd(1+(root<<1),mid,end,pos,val);
		recalc(root);
	}
}
template<class RAI>
void build_tree(int root,RAI begin,RAI end)
{
	if (end-begin==1)
		atomic_upd(root,*begin);
	else
	{
		RAI mid=begin+(end-begin)/2;
		build_tree(root<<1,begin,mid);
		build_tree(1+(root<<1),mid,end);
		recalc(root);
	}
}
int in()
{
	int res=0;
	char c;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		res=(res<<1)+(res<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return res;
}
int main()
{
	int N,i,m,idx,val;
	static int a[200001],b[200001],c[200001],d[200001];
	N=in();
	for (i=1; i<=N; i++)
		b[i]=a[i]=in();
	sort(b+1,b+N+1);
	c[0]=0;
	for (i=1; i<=N; i++)
		c[b[i]]=i;
	for (i=1; i<=N; i++)
		if (!c[i])
			c[i]=c[i-1];
	for (i=1; i<=N; i++)
		d[i]=c[i]-c[i-1]-1;
	build_tree(1,d,d+N+1);
	printf(max_psum[1]>0?"NIE\n":"TAK\n");
	m=in();
	while (m--)
	{
		idx=in(); val=in();
		upd(1,d,d+N+1,d+val,d[val]+1);
		upd(1,d,d+N+1,d+a[idx],d[a[idx]]-1);
		d[val]++;
		d[a[idx]]--;
		a[idx]=val;
		printf(max_psum[1]>0?"NIE\n":"TAK\n");
	}
	return 0;
}
