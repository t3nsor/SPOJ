// 2008-06-04
#include <cstdio>
int main()
{
  int N;
  scanf("%d",&N);
  if (N%10)
    printf("1\n%d\n",N%10);
  else
    printf("2\n");
  return 0;
}
