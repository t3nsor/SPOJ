// 2008-08-19
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
struct st
{
	pair<int,int> l;
	pair<int,int> r;
	int i;
};
st segtree[5000000];
int n;
st join(st left,st right)
{
	st res;
	res.l=left.l;
	res.r=right.r;
	if (left.l.first==right.l.first)
		res.l.second+=right.l.second;
	if (left.r.first==right.r.first)
		res.r.second+=left.r.second;
	res.i=max(max(left.r.second,right.l.second),max(left.i,right.i));
	if (left.r.first==right.l.first)
		res.i=max(res.i,left.r.second+right.l.second);
	return res;
}
void build_segtree(int idx,int l,int r)
{
	if (l==r)
	{
		int x;
		scanf("%d",&x);
		segtree[idx].l=make_pair(x,1);
		segtree[idx].r=make_pair(x,1);
		segtree[idx].i=1;
		return;
	}
	build_segtree(2*idx+1,l,(l+r)/2);
	build_segtree(2*idx+2,(l+r)/2+1,r);
	segtree[idx]=join(segtree[2*idx+1],segtree[2*idx+2]);
}
st query(int idx,int m,int M,int l,int r)
{
	if (l==m&&r==M)
		return segtree[idx];
	st res1;
	st res2;
	if (l<=(m+M)/2)
	{
		res1=query(2*idx+1,m,(m+M)/2,l,min(r,(m+M)/2));
		if (r>(m+M)/2)
		{
			res2=query(2*idx+2,(m+M)/2+1,M,max(l,(m+M)/2+1),r);
			return join(res1,res2);
		}
		return res1;
	}
	return query(2*idx+2,(m+M)/2+1,M,max(l,(m+M)/2+1),r);
}
int main()
{
	int q,l,r;
	st res;
	for(;;)
	{
		scanf("%d",&n);
		if (!n) return 0;
		scanf("%d",&q);
		build_segtree(0,0,n-1);
		while (q--)
		{
			scanf("%d %d",&l,&r);
			res=query(0,0,n-1,l-1,r-1);
			printf("%d\n",max(max(res.l.second,res.r.second),res.i));
		}
	}
}
