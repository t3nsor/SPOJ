// 2009-04-07
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
#define ok(x,y) (x<=y&&!((y-x)%2))
struct st
{
    int x;
    int y;
    int d;
    st(){}
    st(int X,int Y,int D):x(X),y(Y),d(D){}
};
const int dx[]={ 1, 1,-1,-1, 2, 2,-2,-2};
const int dy[]={ 2,-2, 2,-2, 1,-1, 1,-1};
int main()
{
    int T,X,Y,Px,Py,Kx,Ky,i,j,dist[100][100];
    scanf("%d",&T);
    while (T--)
    {
        scanf("%d %d %d %d %d %d",&Y,&X,&Py,&Px,&Ky,&Kx);
        Py--,Px--,Ky--,Kx--;
        //pawn moves to higher y, tries to get to Y-1
        if (Px==Kx&&Ky==Py+1)
        {
            printf("Stalemate in 0 knight move(s).\n");
            continue;
        }
        queue<st> Q;
        Q.push(st(Kx,Ky,0));
        memset(dist,1000000,sizeof(dist));
        while (!Q.empty())
        {
            st P=Q.front();
            Q.pop();
            if (P.x<0||P.x>=X||P.y<0||P.y>=Y)
                continue;
            if (dist[P.x][P.y]<1000000)
                continue;
            dist[P.x][P.y]=P.d;
            for (i=0; i<8; i++)
                Q.push(st(P.x+dx[i],P.y+dy[i],P.d+1));
        }
        bool b=false;
        for (i=Py+1,j=1; i<Y-1; i++,j++) //attempt to capture
            if ok(dist[Px][i],j)
            {
                b=true;
                break;
            }
        if (b)
        {
            printf("Win in %d knight move(s).\n",j);
            continue;
        }
        b=false;
        for (i=Py,j=0; i<Y-1; i++,j++) //attempt to stalemate
            if ok(dist[Px][i+1],j)
            {
                b=true;
                break;
            }
        if (b)
            printf("Stalemate in %d knight move(s).\n",j);
        else
            printf("Loss in %d knight move(s).\n",max(0,Y-Py-2));
    }
	return 0;
}
