// 2023-10-20
// originally written 2008-10-28 but with compilation errors
#include <math.h>
#include <stdio.h>
#define Sin(x) (sin((x)/180.0*3.1415926536))
#define Cos(x) (cos((x)/180.0*3.1415926536))
#define Tan(x) (Sin(x)/Cos(x))
#define Abs(x) ((x)>0?(x):-(x))
int main()
{
	int N,i;
	double D,L,HA,HB,ERRDIST,a,b,c,d,x,y,Z_a,Z_b;
	scanf("%d",&N);
	scanf("%lf %lf %lf %lf %lf",&D,&L,&HA,&HB,&ERRDIST);
	for (i=1; i<=N; i++)
	{
		scanf("%lf %lf %lf %lf",&a,&b,&c,&d);
		if (a<=0||b<=0||c<=0||a>=90||b>=90||c>=90||d<=90||d>=180) //out of bounds
		{
			printf("%d DISQUALIFIED\n",i);
			continue;
		}
		//plane 1: vertical (z-value arbitrary),
		//passes through (0,0) and (cos c, sin c)
		//so equation is (sin c)x - (cos c)y = 0
		//plane 2: also arbitrary z-value
		//passes through (D,0), direction numbers (cos d, sin d)
		//so equation is (sin d)x - (cos d)y - D sin d = 0
		//intersection:
		//(sin c) x - (cos c) y = 0
		//(sin d) x - (cos d) y = D sin d
		//(sin c sin d) x - (cos c sin d) y = 0
		//(sin c sin d) x - (sin c cos d) y = D sin c sin d
		//(cos c sin d) y - (sin c cos d) y = D sin c sin d
		//y = (D sin c sin d)/(cos c sin d - sin c cos d)
		/*y = (D sin c sin d)/sin(d - c)
		  x = (D cos c sin d)/sin(d - c)*/
		x=D*Cos(c)*Sin(d)/Sin(d-c);
		y=D*Sin(c)*Sin(d)/Sin(d-c);
		//and for the line from A, the elevation angle is a
		//so we define a plane... with satisfies z/sqrt(x^2+y^2) = tan a
		//or z = sqrt(x^2+y^2) tan a
		//and for the line from B, the elevation angle is b
		//another plane... here z/sqrt((D-x)^2+y^2) = tan b
		Z_a=sqrt(x*x+y*y)*Tan(a)+HA;
		Z_b=sqrt((D-x)*(D-x)+y*y)*Tan(b)+HB;
		if (Abs(Z_a-Z_b)>ERRDIST)
			printf("%d ERROR\n",i);
		else
			printf("%d %.0f\n",i,(Z_a+Z_b)/2.0);
	}
	return 0;
}
