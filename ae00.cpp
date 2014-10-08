// 2009-12-21
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
     int N,res=0,i,s;
     scanf("%d",&N);
     s=sqrt(N);
     for (i=1; i<=s; i++)
          res+=N/i-i+1;
     printf("%d\n",res);
     return 0;
}
