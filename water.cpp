// 2009-02-10
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
typedef pair<int,pair<int,int> > PIII;
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
int main()
{
    int T,R,C,i,j,r,c;
    int ht[100][100];
    bool vis[100][100];
    int res;
    scanf("%d",&T);
    while (T--)
    {
        priority_queue<PIII,vector<PIII>,greater<PIII> > Q;
        scanf("%d %d",&R,&C);
        res=0;
        for (i=0; i<R; i++)
            for (j=0; j<C; j++)
            {
                scanf("%d",ht[i]+j);
                vis[i][j]=false;
                if (i==0||j==0||i+1==R||j+1==C)
                    Q.push(PIII(ht[i][j],make_pair(i,j)));
            }
        while (!Q.empty())
        {
            PIII P=Q.top();
            while (!Q.empty())
            {
                PIII P2=Q.top();
                if (P2.first>P.first) break;
                Q.pop();
                if (vis[P2.second.first][P2.second.second]++)
                    continue;
                res+=P.first-P2.first;
                for (i=0; i<4; i++)
                {
                    r=P2.second.first+dx[i];
                    c=P2.second.second+dy[i];
                    if (r>=0&&r<R&&c>=0&&c<C)
                        Q.push(PIII(ht[r][c],make_pair(r,c)));
                }
            }
        }
        //		fprintf(stderr,"%d %d\n",R,C);
        printf("%d\n",res);
    }
    return 0;
}
