// 2009-07-09
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
const int empty=0; //empty submatrix doesn't count
static int BITPool[32000][300]={0}; //38 MB!
int* BIT=BITPool[0];
int sz;
int get(int x)
{
	int res=0;
	for (; ~x; x=(x&(x+1))-1)
		res+=BIT[x];
	return res;
}
void set(int x)
{
	int res=0;
	for (; x<sz; x|=x+1)
		BIT[x]++;
}
int main()
{
	int R,C,i,j,k,m,A,B,l,u,cur;
	int a[300][300],s[300][300];
	int b[300],c[300];
#ifndef ONLINE_JUDGE
	freopen("kpmatrix.in","r",stdin);
#endif
	scanf("%d %d",&R,&C);
	for (i=0; i<R; i++)
		for (j=0; j<C; j++)
			scanf("%d",a[i]+j);
	scanf("%d %d",&A,&B);
	//cache partial sums
	//s[x][y]=sum up-to-but-not-including col x in row y
	for (i=0; i<R; i++)
	{
		s[0][i]=0;
		for (j=1; j<=C; j++)
			s[j][i]=s[j-1][i]+a[i][j-1];
	}
	int res=empty*(A<=0&&B>=0); //does the empty submatrix count?
	for (i=0; i<C; i++)
		for (j=i; j<C; j++)
		{
			//b[]: actual values of interest
			//c[]: partial sums of b[]
			c[0]=0;
			for (k=0; k<R; k++)
			{
				b[k]=s[j+1][k]-s[i][k];
				c[k+1]=c[k]+b[k];
			}
			c[R+1]=-2.1e9;
			sort(c,c+R+2);
			//eliminate duplicates
			k=0;
			for (m=0; m<R+2; m++)
				if (m==0||c[m]!=c[m-1])
					c[k++]=c[m];
			sz=k;
			cur=0;
			set(lower_bound(c,c+sz,0)-c);
			for (k=0; k<R; k++)
			{
				cur+=b[k];
				//A<=cur-s<=B
				//or: -A>=s-cur>=-B
				//or: cur-A>=s>=cur-B
				//or: cur-B<=s<=cur-A
				l=lower_bound(c,c+sz,cur-B)-c-1;
				u=upper_bound(c,c+sz,cur-A)-c-1;
				res+=get(u)-get(l);
				set(lower_bound(c,c+sz,cur)-c);
			}
			BIT+=300;
		}
	printf("%d\n",res);
	return 0;
}
