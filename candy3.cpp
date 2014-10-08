// 2008-06-11
#include <cstdio>
long long input()
{
  long long x=0;
  char c;
  for(;;)
  {
    c=getchar_unlocked();
    if (c=='\n') return x;
    x=10*x+c-'0';
  }
  return x;
}
int main()
{
  int T,i,N,j;
  long long sum,c;
  T=input();
  for (i=0; i<T; i++)
  {
    getchar_unlocked();
    N=input();
    sum=0;
    for (j=0; j<N; j++)
    {
      c=input();
      sum=(sum+c%N)%N;
    }
    if (sum)
      printf("NO\n");
    else
      printf("YES\n");
  }
  return 0;
}
