// 2010-11-13
/*
ID: !woburn1
PROG: cattleb
LANG: C++
*/
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;
int main()
{
	int N,R,dx,dy,vx,vy,DX,DY,VX,VY,i,cnt=0,best=0,e;
	static bool inrange[100000]={0};
	long long a,b,c,d; //d potentially overflows int; making a,b,c long long as well simplifies the code
	double t1,t2;
	static pair<double,int> A[200000];
	scanf("%d %d %d %d %d %d",&N,&R,&dx,&dy,&vx,&vy);
	//N = # of cows; R = radius; (dx,dy) = initial position; (vx,vy) = initial velocity
	for (i=0,e=0; i<N; i++) //e = # of "events"
	{
		scanf("%d %d %d %d",&DX,&DY,&VX,&VY);
		//(DX,DY) = initial position; (VX,VY) = initial velocity
		//now: "transform" these coordinates so that Bessie is still at (0,0)
		//no acceleration; use Galilean transformation equations
		//fairly intuitive
		DX-=dx; DY-=dy; VX-=vx; VY-=vy;
		//at time t: bruiser i is at position (DX+tVX,DY+tVY) (t>=0)
		//squared distance from origin = (DX+tVX)^2 + (DY+tVY)^2
		//when does it equal R^2?
		//DX^2 + t^2 VX^2 + 2DXtVX + DY^2 + t^2 VY^2 + 2DYtVY = R^2
		//t^2 (VX^2 + VY^2) + t (2DXVX + 2DYVY) + (DX^2 + DY^2 - R^2) = 0
		a=VX*VX+VY*VY;     // ||v||
		b=2*(DX*VX+DY*VY); // 2(d&#183;v)
		c=DX*DX+DY*DY-R*R; // ||d||-R^2;
		d=b*b-4*a*c;       // discriminant of quadratic
		//if a = 0, then the bruiser doesn't move
		//this messes up the quadratic formula; but the bruiser stays stationary relative to Bessie
		//if d < 0, then crossing from within range to outside range or vice versa doesn't occur
		if (a==0||d<0)
		{
			if (c<=0) //within range
				cnt++;
			continue;
		}
		//otherwise: a!=0 and d>=0
		t1=max((-b-sqrt(d))/(2.0*a),0.0);
		t2=(-b+sqrt(d))/(2.0*a);
		//enters range at t1; exits at t2
		if (t2>=0.0) //in range for some point (t>=0)
		{
if (e>=200000) throw;
			A[e++]=make_pair(t1,i);
if (e>=200000) throw;
			A[e++]=make_pair(t2,i);
		}
		//otherwise: within range only for t < 0
		//note: if t1 is negative and t2 isn't, then the bruiser is already within range at t=0; hence max(root,0)
	}
	best=cnt;
	sort(A,A+e);
	for (i=0; i<e; i++)
	{
		if (inrange[A[i].second])
			cnt--;
		else
		{
			inrange[A[i].second]=true;
			cnt++;
			best=max(cnt,best);
		}
	}
	printf("%d\n",best);
	return 0;
}
