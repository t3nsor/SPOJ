// 2018-04-14
#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;
int gcd(int x,int y)
{
    if (x==0) return y;
    return gcd(y%x,x);
}
int main()
{
    int T,a,b,c;
    scanf("%d",&T);
    while (T--)
    {
        scanf("%d %d %d",&a,&b,&c);
        if (c%gcd(a,b)||c>max(a,b))
            puts("-1");
        else
        {
            int best=2e9;
            int A=0;
            int B=0;
            int moves=0;
            while (A!=c&&B!=c)
            {
                moves++;
                if (A==0) A=a;
                else if (B==b) B=0;
                else
                {
                    int trans=min(A,b-B);
                    A-=trans;
                    B+=trans;
                }
            }
            best=min(best,moves);
            A=0; B=0; moves=0;
            moves=0;
            while (A!=c&&B!=c)
            {
                moves++;
                if (B==0) B=b;
                else if (A==a) A=0;
                else
                {
                    int trans=min(B,a-A);
                    B-=trans;
                    A+=trans;
                }
            }
            best=min(best,moves);
            printf("%d\n",best);
        }
    }
    return 0;
}
