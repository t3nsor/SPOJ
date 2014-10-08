// 2008-08-19
#include <iostream>
using namespace std;
int n;
int BIT[20000];
int index(int num)
{
	int x;
	for (x=num-1; x>=0; x=(x&(x+1))-1)
		num-=BIT[x];
	return num;
}
int query(int pos)
{
	int l=0;
	int u=n-1;
	int m=0;
	while (u>l)
	{
		m=(l+u)/2+1;
		if (index(m)>pos)
			u=m-1;
		else
			l=m;
	}
	return l;
}
void del(int num)
{
	for (;num<n;num|=num+1)
		BIT[num]++;
}
int main()
{
	int T,i,pos;
	int arr[20000];
	scanf("%d",&T);
	while (T--)
	{
		memset(BIT,0,sizeof(BIT));
		scanf("%d",&n);
		pos=0;
		for (i=1; i<=n; i++)
		{
			pos=(pos+i)%(n-i+1);
			int q=query(pos);
			arr[q]=i;
			del(q);
		}
		for (i=0; i<n-1; i++)
			printf("%d ",arr[i]);
		printf("%d\n",arr[n-1]);
	}
	return 0;
}
