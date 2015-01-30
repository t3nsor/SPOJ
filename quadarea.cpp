// 2008-05-31
#include <cstdio>
#include <cmath>
int main()
{
     int T,i;
     long double a,b,c,d,s,r;
     scanf("%d",&T);
     for (i=0; i<T; i++)
     {
          scanf("%llf %llf %llf %llf",&a,&b,&c,&d);
          s=(a+b+c+d)/2.0;
          r=sqrt((s-a)*(s-b)*(s-c)*(s-d));
          printf("%llf\n",r);
     }
     return 0;
}
