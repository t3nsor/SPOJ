// 2008-12-04
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
struct Vec
{
	double x;
	double y;
	Vec(){}
	Vec(double X,double Y):x(X),y(Y){}
};
inline Vec operator-(Vec v1,Vec v2)
{
	return Vec(v1.x-v2.x,v1.y-v2.y);
}
inline double operator*(Vec v1,Vec v2) //det
{
	return v1.x*v2.y-v1.y*v2.x;
}
inline bool operator<(Vec v1,Vec v2) //used by sort()
{
	return atan2(v1.y,v1.x)<atan2(v2.y,v2.x);
}
inline double SQR(double x)
{
	return x*x;
}
inline double DIST(Vec v1,Vec v2)
{
	return sqrt(SQR(v1.x-v2.x)+SQR(v1.y-v2.y));
}
bool col(int X1,int Y1,int X2,int Y2,int X3,int Y3)
{
	if ((X3-X2)*(Y2-Y1)==(Y3-Y2)*(X2-X1))
		return true;
	else
		return false;
}
int main()
{
	int N,i,n,T;
	Vec p[100000];
	int ch[100000];
	for (i=0; i<T; i++)
	{
		scanf("%d",&N);
		long double sumx=0,sumy=0;
		for (i=0; i<N; i++)
		{
			scanf("%d %d",&X[i],&Y[i]);
			p[i]=Vec(X[i],Y[i]);
			sumx+=p[i].x,sumy+=p[i].y;
		}
		Vec v(sumx/N,sumy/N);
		for (i=0; i<N; i++)
			p[i]=p[i]-v;
		sort(p,p+N);
		n=2;
		ch[0]=0;
		ch[1]=1;
		for (i=2; i<N; i++)
		{
			ch[n]=i;
			while (n>1 && ((p[ch[n]]-p[ch[n-1]])*(p[ch[n-2]]-p[ch[n-1]])<0 ||
				col(X[ch[n]],Y[ch[n]],X[ch[n-1]],Y[ch[n-1]],X[ch[n-2]],Y[ch[n-2]])))
				ch[--n]=ch[n+1];
			n++;
		}
		int begin=0;
		int end=n-1;
		bool b;
		do
		{
			b=false;
			while ((p[ch[begin]]-p[ch[end]])*(p[ch[end-1]]-p[ch[end]])<0 ||
				col(X[ch[begin]],Y[ch[begin]],X[ch[end]],Y[ch[end]],X[ch[end-1]],Y[ch[end-1]]))
				end--,b=true;
			while ((p[ch[begin+1]]-p[ch[begin]])*(p[ch[end]]-p[ch[begin]])<0 ||
				col(X[ch[begin+1]],Y[ch[begin+1]],X[ch[begin]],Y[ch[begin]],X[ch[end]],Y[ch[end]]))
				begin++,b=true;
		}
		while (b);
		double dist=0.0;
		for (i=begin; i<end; i++)
			dist+=DIST(p[ch[i]],p[ch[i+1]]);
		dist+=DIST(p[ch[end]],p[ch[begin]]);
		printf("%.2lf\n",dist);
		int m=0;
		for (i=begin+1; i<end; i++)
			if (Y[ch[i]]<Y[ch[m]]||Y[ch[i]]==Y[ch[m]]&&X[ch[i]]<X[ch[m]])
				m=i;
		for (i=m; i<end; i++)

	}
	return 0;
}
