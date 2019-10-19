#include<bits/stdc++.h>
using namespace std;
list<int>adj[1000001];
int visited[1000001],lv[1000001];
bool flag[1000001];
bool bfs(int src,int st,int v)
{
    queue<int> q;
    q.push(src);
    lv[src]=0;
    visited[src]=v;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        if(st>=lv[u] && flag[u])
            return false;
        if(st>=lv[u] && !flag[u])
            flag[u]=true;
        if(st<lv[u])
            return true;
        for(auto it=adj[u].begin();it!=adj[u].end();it++)
        {
            if(visited[*it]!=v)
            {
                lv[*it]=lv[u]+1;
                visited[*it]=v;
                q.push(*it);
            }
        }
    }
return true;
}
int main()
{
    int t,n,r,m,k,l,u,v;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d%d",&n,&r,&m);
        for(int i=1;i<=r;i++)
        {
            scanf("%d%d",&u,&v);
            adj[u].push_back(v),adj[v].push_back(u);
        }
        bool y=true;
        for(int i=1;i<=m;i++)
        {
            scanf("%d%d",&k,&l);
            if(y)
                y=bfs(k,l,i);
        }
        if(!y)
            printf("No\n");
        else
            printf((count(flag+1,flag+n+1,false)==0?"Yes\n":"No\n"));
        for(int i=1;i<=n;i++)
            visited[i]=0,adj[i].clear(),flag[i]=false;
    }
}
