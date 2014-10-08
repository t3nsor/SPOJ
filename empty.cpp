// 2008-07-31
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <set>
#include <algorithm>
using namespace std;
struct Point
{
	int x;
	int y;
	Point(){}
	Point(int X,int Y):x(X),y(Y){}
};
bool operator<(const Point& p1,const Point& p2)
{
	return p1.x<p2.x;
}
bool operator>(const Point& p1,const Point& p2)
{
	long long a1=(long long)p1.x*p1.y;
	long long a2=(long long)p2.x*p2.y;
	return a1>a2||a1==a2&&p1.x<p2.x;
}
struct Point3D
{
	int x;
	int y;
	int z;
};
bool operator<(Point3D p1,Point3D p2)
{
	return p1.z<p2.z;
}
set<Point,less<Point> > S; //to hold all points, ordered by x-coordinate
set<Point,greater<Point> > T; //to hold all points, ordered by x*y, then by x
bool find(Point p)
{
	set<Point,less<Point> >::iterator I=S.upper_bound(Point(p.x,2000000));
	if (I==S.begin())
		return false;
	//if I is S.begin, then p is extreme-left.
	I--;
	//last point with x-coordinate <= p.x
	if (I->y<=p.y)
		return true;
	else
		return false;
}
void insert(Point p)
{
	if (find(p)) //no regular cuboid is bounded by this point
		return;
	//A point will remain in the set if it has either a lower x-coordinate or a lower
	//y-coordinate than point p. Otherwise, it will be removed.
	//Point p will be added to the set since it has passed the test.
	//Rectangles which become invalid due to points being removed, will be deleted
	//from the set.
	set<Point,less<Point> >::iterator I=S.lower_bound(Point(p.x,2000000)),J=I,K=I;
	//finds the smallest x-coordinate >= p.x
	//delete the rectangle bounded by the point "just before" this one, if possible
	if (K!=S.begin()&&K!=S.end())
	{
		int x=K->x;
		K--;
		T.erase(Point(x,K->y));
	}
	while (J!=S.end()&&J->y>=p.y)
	{
		int y=J->y;
		J++;
		if (J!=S.end())
			T.erase(Point(J->x,y));
	}
	S.erase(I,J);
	S.insert(p);
	I=S.find(p);
	if (I!=S.begin())
	{
		I--;
		T.insert(Point(p.x,I->y));
		I++;
	}
	I++;
	if (I!=S.end())
		T.insert(Point(I->x,p.y));
}
int main()
{
	int t,N,i;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d",&N);
		S.clear();
		T.clear();
		//takes advantage of unusual operator overloading
		Point3D points[5003];
		for (i=0; i<N; i++)
			scanf("%d %d %d",&points[i].x,&points[i].y,&points[i].z);
		points[N].x=0; points[N].y=1000000; points[N].z=0;
		points[N+1].x=1000000; points[N+1].y=0; points[N+1].z=0;
		points[N+2].x=0; points[N+2].y=0; points[N+2].z=1000000;
		N+=3;
		sort(points,points+N);
		long long volume=0;
		Point3D best;
		Point P(0,0);
		for (i=0;;i++)
		{
			if ((long long)P.x*P.y*points[i].z>volume)
			{
				volume=(long long)P.x*P.y*points[i].z;
				best.x=P.x;
				best.y=P.y;
				best.z=points[i].z;
			}
			if (points[i].z==1000000)
				break;
			insert(Point(points[i].x,points[i].y));
			if (T.size())
				P=*(T.begin());
		}
		printf("%d %d %d\n",best.x,best.y,best.z);
	}
	return 0;
}
