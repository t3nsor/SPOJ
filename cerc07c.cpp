// 2008-09-26
#include <iostream>
#include <algorithm>
#include <cmath>
#define DIST(x1,y1,x2,y2) sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
#define ABS(x) ((x)>0?x:-(x))
#define PI 3.1415926536
using namespace std;
int main()
{
	pair<double,int> a[4000];
	int N,i,j;
	double x[2000];
	double y[2000];
	bool b[2000];
	double R,cx,cy;
	for(;;)
	{
		scanf("%d %lf",&N,&R);
		R+=0.0005;
		if (N==0) return 0;
		for (i=0; i<N; i++)
			scanf("%lf %lf",x+i,y+i);
		int res=0;
		for (i=0; i<N; i++)
		{
			memset(b,false,sizeof(b));
			int cur=0;
			cx=x[i]+R;
			cy=y[i];
			int cnt=0;
			for (j=0; j<N; j++)
			{
				if (DIST(cx,cy,x[j],y[j])<R+0.0005)
				{
					b[j]=true;
					cur++;
				}
				if (i==j) continue;
				double dist=DIST(x[i],y[i],x[j],y[j])/2.0;
				if (dist>R) continue;
				double dist2=sqrt(R*R-dist*dist);
				double xdiff=x[i]-x[j];
				double ydiff=y[i]-y[j];
				dist*=2.0;
				double x1=(x[i]+x[j])/2.0-ydiff/dist*dist2-x[i];
				double x2=(x[i]+x[j])/2.0+ydiff/dist*dist2-x[i];
				double y1=(y[i]+y[j])/2.0+xdiff/dist*dist2-y[i];
				double y2=(y[i]+y[j])/2.0-xdiff/dist*dist2-y[i];
				double a1=atan2(y1,x1); if (a1<0) a1+=2*PI;
				double a2=atan2(y2,x2); if (a2<0) a2+=2*PI;
				if (ABS(a1-a2)>1e-6)
				{
					a[cnt++]=make_pair(a1,j);
					a[cnt++]=make_pair(a2,j);
				}
				else
					a[cnt++]=make_pair(a1,j);
			}
			if (res<cur) res=cur;
			sort(a,a+cnt);
			for (j=0; j<cnt; j++)
			{
				if (b[a[j].second])
				{
					cur--;
					b[a[j].second]=false;
				}
				else
				{
					cur++;
					if (res<cur) res=cur;
					b[a[j].second]=true;
				}
			}
		}
		printf("It is possible to cover %d points.\n",res);
	}
	return 0;
}
