// 2010-12-11
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <deque>
using namespace std;
int dx[]={0,1,0,-1};
int dy[]={1,0,-1,0};
struct S {int r; int c; int dir; int dist; };
int main()
{
    int R,C,i,j;
    char grid[111][111];
    int r1,r2,c1,c2;
    int dist[111][111][4];
    scanf("%d %d",&C,&R);
    for (i=0; i<R; i++)
    {
	scanf("%s",grid+i);
	for (j=0; j<C; j++)
	    if (grid[i][j]=='C')
	    {
		r1=i;
		c1=j;
		swap(r1,r2);
		swap(c1,c2);
	    }
    }
    deque<S> D;
    memset(dist,0x3f,sizeof(dist));
    D.push_back((S){r1,c1,0,0});
    D.push_back((S){r1,c1,1,0});
    D.push_back((S){r1,c1,2,0});
    D.push_back((S){r1,c1,3,0});
    for (;;)
    {
	S s=D.front();
	D.pop_front();
	if (s.r<0||s.r>=R||s.c<0||s.c>=C||grid[s.r][s.c]=='*'||dist[s.r][s.c][s.dir]<=s.dist)
	    continue;
	if (s.r==r2&&s.c==c2)
	{
	    printf("%d\n",s.dist);
	    return 0;
	}
	dist[s.r][s.c][s.dir]=s.dist;
	for (i=0; i<4; i++)
	    if (i==s.dir)
		D.push_front((S){s.r+dx[i],s.c+dy[i],i,s.dist});
	    else
		D.push_back((S){s.r+dx[i],s.c+dy[i],i,s.dist+1});
    }
}
