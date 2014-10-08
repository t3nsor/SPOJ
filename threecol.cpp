// 2008-07-17
// modified 2014-10-07
#include <cstdio>
#include <algorithm>
using namespace std;
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
const int SIZE = 20000;
int N;
char s[SIZE+1];
int ch[SIZE];
int lch[SIZE];
int rch[SIZE];
int maxg[SIZE][3];
int ming[SIZE][3];
bool memo[SIZE];
void readtree()
{
	int n=N;
	char c=s[N];
	if (c=='0')
		ch[N++]=0;
	else if (c=='1')
	{
		ch[N++]=1;
		lch[n]=N;
		readtree();
	}
	else
	{
		ch[N++]=2;
		lch[n]=N;
		readtree();
		rch[n]=N;
		readtree();
	}
}
void f(int x)
{
	if (memo[x]) return;
	memo[x]=true;
	if (ch[x]==0) //no children
	{
		maxg[x][0]=1; //green
		maxg[x][1]=0; //red
		maxg[x][2]=0; //blue
		ming[x][0]=1; //green
		ming[x][1]=0; //red
		ming[x][2]=0; //blue
		return;
	}
	int l=lch[x],r=rch[x];
	f(l);
	if (ch[x]==1)
	{
		maxg[x][0]=1+max(maxg[l][1],maxg[l][2]);
		maxg[x][1]=max(maxg[l][0],maxg[l][2]);
		maxg[x][2]=max(maxg[l][0],maxg[l][1]);
		ming[x][0]=1+min(ming[l][1],ming[l][2]);
		ming[x][1]=min(ming[l][0],ming[l][2]);
		ming[x][2]=min(ming[l][0],ming[l][1]);
	}
	if (ch[x]==2)
	{
		f(r);
		maxg[x][0]=1+max(maxg[l][1]+maxg[r][2],maxg[l][2]+maxg[r][1]);
		maxg[x][1]=max(maxg[l][0]+maxg[r][2],maxg[l][2]+maxg[r][0]);
		maxg[x][2]=max(maxg[l][0]+maxg[r][1],maxg[l][1]+maxg[r][0]);
		ming[x][0]=1+min(ming[l][1]+ming[r][2],ming[l][2]+ming[r][1]);
		ming[x][1]=min(ming[l][0]+ming[r][2],ming[l][2]+ming[r][0]);
		ming[x][2]=min(ming[l][0]+ming[r][1],ming[l][1]+ming[r][0]);
	}
}
int main()
{
	int i,j,t;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		N=0;
		scanf("%s",&s);
		readtree();
		for (j=0; j<N; j++)
			memo[j]=false;
		f(0);
		printf("%d %d\n",max(max(maxg[0][0],maxg[0][1]),maxg[0][2]),min(min(ming[0][0],ming[0][1]),ming[0][2]));
	}
	return 0;
}
