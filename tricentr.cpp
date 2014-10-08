// 2008-10-09
#include<stdio.h>
#include<math.h>

double round1(double value, unsigned int decimals)
{
  double factor = pow(10,decimals);
  return floor((value * factor) + 0.5) / factor;
}

int main()
{
    int t,i; double a,b,c,a1,b1,c1,s,R,A,OH,GH,exp;
    scanf("%d",&t);
   
    for(i=0;i<t;i++)
    {
                    scanf("%lf%lf%lf%lf",&a,&a1,&b1,&c1);
                    A= 1.50*a*a1;
                    b= A/(1.50*b1);
                    c= A/(1.50*c1);
                    s=(a+b+c)/2;
                    R= a*b*c / ( 4 * sqrt( s*(a+b-s)*(a+c-s)*(b+c-s)));
                    exp= ((9*R*R-a*a-b*b-c*c));
                    if(exp<0)
                    exp*=-1;
                    OH=sqrt(exp);
                    GH= (2/3.0)*OH;
                    printf("%.3lf %.3lf\n",A,GH);
    }


    return 0;
   
} 
