// 2009-08-06
/*
ID: brian_bi21
PROG: fence3
LANG: C++
*/
#include <cstdio>
#include <cmath>
int hf;
int vf;
int xbegin[150];
int xend[150];
int xval[150];
int ybegin[150];
int yend[150];
int yval[150];
void swap(int& x,int& y)
{
	int t=x;
	x=y;
	y=t;
}
inline double ABS(double x)
{
	return x>0.0?x:-x;
}
inline double SQR(double x)
{
	return x*x;
}
inline double MIN(double x,double y)
{
	return x<y?x:y;
}
double dist(double x,double y)
{
	double res=0.0;
	int i;
	for (i=0; i<hf; i++)
		if (x>=xbegin[i]&&x<=xend[i])
			res+=ABS(y-yval[i]);
		else if (x<xbegin[i])
			res+=sqrt(SQR(xbegin[i]-x)+SQR(y-yval[i]));
		else
			res+=sqrt(SQR(x-xend[i])+SQR(y-yval[i]));
	for (i=0; i<vf; i++)
		if (y>=ybegin[i]&&y<=yend[i])
			res+=ABS(x-xval[i]);
		else if (y<ybegin[i])
			res+=sqrt(SQR(x-xval[i])+SQR(ybegin[i]-y));
		else
			res+=sqrt(SQR(x-xval[i])+SQR(y-yend[i]));
	return res;
}
int main()
{
//	freopen("fence3.in","r",stdin);
//	freopen("fence3.out","w",stdout);
	int i,F;
	scanf("%d",&F);
	hf=0;
	vf=0;
	for (i=0; i<F; i++)
	{
		int x,y,X,Y;
		scanf("%d %d %d %d",&x,&y,&X,&Y);
		if (x>X) swap(x,X);
		if (y>Y) swap(y,Y);
		if (x==X) //vertical fence
		{
			ybegin[vf]=y;
			yend[vf]=Y;
			xval[vf]=x;
			vf++;
		}
		else
		{
			xbegin[hf]=x;
			xend[hf]=X;
			yval[hf]=y;
			hf++;
		}
	}
	double x=50.0;
	double y=50.0;
	double diff=50.0;
	while (diff>0.01)
	{
		double d1=dist(x-diff,y-diff);
		double d2=dist(x+diff,y-diff);
		double d3=dist(x-diff,y+diff);
		double d4=dist(x+diff,y+diff);
		double m=MIN(MIN(d1,d2),MIN(d3,d4));
		if (m==d1||m==d3)
			x-=diff;
		else
			x+=diff;
		if (m==d1||m==d2)
			y-=diff;
		else
			y+=diff;
		diff/=1.4; //magic constant
	}
	printf("%.1lf %.1lf %.1lf\n",x,y,dist(x,y));
	return 0;
}
