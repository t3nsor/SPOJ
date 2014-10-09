// 2011-12-11
// nhay-2.cpp uses KMP. For Rabin-Karp, see nhay-1.cpp
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
int in()
{
    int res=0;
    int c;
    do
    {
        c=getchar_unlocked();
        if (c==EOF) return 0;
    }
    while (c<=32);
    do
    {
        res=10*res+(c-'0');
        c=getchar_unlocked();
    }
    while (c>32);
    return res;
}
int main()
{
    int n,k,i;
    char c;
    static char needle[1000000];
    static int pi[1000000];
    while (n=in())
    {
        gets(needle);
        pi[1]=0;
        k=0;
        for (i=2; i<=n; i++)
        {
            while (k>0&&needle[k]!=needle[i-1])
                k=pi[k];
            if (needle[k]==needle[i-1])
                k++;
            pi[i]=k;
        }
        k=0;
        i=0;
        while ((c=getchar_unlocked())!='\n')
        {
            i++;
            while (k>0&&needle[k]!=c)
                k=pi[k];
            if (needle[k]==c)
                k++;
            if (k==n)
            {
                printf("%d\n",i-n);
                k=pi[k];
            }
        }
        putchar('\n');
    }
    return 0;
}
