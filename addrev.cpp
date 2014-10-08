// 2007-11-16
#include <stdio.h>

int digits;
int i;
int d[15];

inline int rev(int n)
{
     int digits;
     int i;
     int d[15];
     if (n==0) return 0;
     digits=0;
     while (n>0)
     {
          d[digits++]=n%10;
          n/=10;
     }
     n=0;
     for (i=0; i<digits; i++)
          n=10*n+d[i];
	 return n;
}
int main()
{
     int N,j,x,y;
     scanf("%d",&N);
     for (j=0; j<N; j++)
     {
          scanf("%d %d",&x,&y);
          x=rev(x);
          y=rev(y);
          printf("%d\n",rev(x+y));
     }
     return 0;
}
