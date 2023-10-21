// 2023-10-20
// originally written 2008-09-26 but with compilation errors
#include <algorithm>
#include <stdio.h>
#include <utility>
int in()
{
    char c;
    int res=0;
    bool minus=false;
    for(;;)
    {
        c=getchar_unlocked();
        if (c=='-')
            minus=true;
        else if (c<=32)
            return minus?-res:res;
        else
            res=(res<<1)+(res<<3)+c-'0';
    }
}
using namespace std;
int main()
{
    pair<pair<int,int>,int> A[1000];
    pair<pair<int,int>,int> B[1000];
    int px[1000];
    int py[1000];
    int nx[1000];
    int ny[1000];
    bool up[1000];
    bool right[1000];
    char res[1000];
    int N,i;
    for(;;)
    {
        N=in();
        if (!N) return 0;
        for (i=0; i<N; i++)
        {
            A[i].first.first=B[i].first.second=px[i]=in();
            B[i].first.first=A[i].first.second=py[i]=in();
            A[i].second=B[i].second=i;
        }
        getchar_unlocked();
        sort(A,A+N);
        sort(B,B+N);
        for (i=0; i<(N>>1); i++)
        {
            ny[A[i<<1].second]=A[(i<<1)+1].second;
            up[A[i<<1].second]=true;
            ny[A[(i<<1)+1].second]=A[i<<1].second;
            up[A[(i<<1)+1].second]=false;
            nx[B[i<<1].second]=B[(i<<1)+1].second;
            right[B[i<<1].second]=true;
            nx[B[(i<<1)+1].second]=B[i<<1].second;
            right[B[(i<<1)+1].second]=false;
        }
        int cnt=0;
        int firstpoint;
        bool v=true;
        int point=A[0].second;
        while (cnt<N)
        {
            if (!point)
                firstpoint=cnt;
            if (v)
            {				
                if (up[point])
                    res[cnt++]='N';
                else
                    res[cnt++]='S';
                v=false;
                point=ny[point];
            }
            else
            {
                if (right[point])
                    res[cnt++]='E';
                else
                    res[cnt++]='W';
                v=true;
                point=nx[point];
            }
        }
        for (i=firstpoint; i<N; i++)
            putchar(res[i]);
        for (i=0; i<firstpoint; i++)
            putchar(res[i]);
        putchar('\n');
    }
    return 0;
}
