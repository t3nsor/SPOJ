// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int depends_on_howmany[100];
int depend_on_this[100][100];
int howmany_depend_on_this[100];
int main()
{
	int N,M,i,j,x,y,z;
	scanf("%d %d",&N,&M);
	//memset(exist,false,N);
	for (i=0; i<M; i++)
	{
		scanf("%d %d",&x,&y); x--;
		depends_on_howmany[x]=y;
		for (j=0; j<y; j++)
		{
			scanf("%d",&z); z--;
			depend_on_this[z][howmany_depend_on_this[z]++]=x;
		}
	}
	priority_queue<int,vector<int>,greater<int> > Q;
	for (i=0; i<N; i++)
		if (!depends_on_howmany[i])
			Q.push(i);
	int cnt=0;
	while (cnt<N)
	{
		int x=Q.top();
		Q.pop();
		cnt++;
		if (cnt<N)
			printf("%d ",x+1);
		else
			printf("%d\n",x+1);
		for (i=0; i<howmany_depend_on_this[x]; i++)
			if (depends_on_howmany[depend_on_this[x][i]]--==1)
				Q.push(depend_on_this[x][i]);
	}
	return 0;
}
