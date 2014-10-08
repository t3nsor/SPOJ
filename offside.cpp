// 2008-06-13
#include <cstdio>
int input()
{
  int x=0;
  char c;
  for(;;)
  {
    c=getchar_unlocked();
    if (c==' '||c=='\n')
      return x;
    x=10*x+c-'0';
  }
}
int main()
{
  int i,A,D,mina,mind,min2d,x;
  for(;;)
  {
    A=input();
    D=input();
    if (A==0) return 0;
    mina=1000000;
    mind=1000000;
    min2d=1000000;
    for (i=0; i<A; i++)
    {
      x=input();
      if (mina>x)
          mina=x;
    }
    for (i=0; i<D; i++)
    {
      x=input();
      if (x<mind)
      {
        min2d=mind;
        mind=x;
      }
      else if (x<min2d)
        min2d=x;
    }
    if (D==1)
      putchar_unlocked('Y');
    else if (mina<min2d)
      putchar_unlocked('Y');
    else
      putchar_unlocked('N');
    putchar_unlocked('\n');
  }
}
