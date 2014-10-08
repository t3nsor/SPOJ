// 2008-10-24
#include <iostream>
#include <set>

using namespace std;

#define INF 99999
#define MAXN 20001

int main () {
   int N;
   int i,j;
   float slope,x1,y1;
   int x[MAXN],y[MAXN];

for(;;)
{
   scanf("%d",&N);
   if (!N) return 0;
   set<float> s;
   for(i=0;i<N;i++) 
      scanf("%d %d",x+i,y+i);

   for(i=0;i<N;i++)
      for(j=i+1;j<N;j++) {
         y1 = y[i] - y[j];
         x1 = x[i] - x[j];   
         if(x1==0) slope = INF;
         else slope = y1/x1;
         s.insert(slope);
      }   
   int ans = int(s.size());
   printf("%d\n",ans);
}
}
