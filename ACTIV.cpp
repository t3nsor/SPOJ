#include<bits/stdc++.h>
using namespace std;
#define MOD 100000000

bool compare(pair<long long int,long long int>a,pair<long long int,long long int> b)
{
    if(a.second<b.second)
        return true;
    if(a.second==b.second)
        if(a.first<b.first)
            return true;

    return false;

}

int main()
{
    int n;
    cin>>n;

    while(n!=-1)
    {
        pair<long long int,long long int>p[n];
        for(int i=0;i<n;i++)
        {
            cin>>p[i].first;
            cin>>p[i].second;
        }

        sort(p,p+n,compare);

        /*for(int i=0;i<n;i++)
        {
            cout<<p[i].first<<" "<<p[i].second<<endl;
        }*/

        long long dp[n];
        memset(dp,0,sizeof(dp));
        dp[0]=1;

        for(int i=1;i<n;i++)
        {
            dp[i]=dp[i-1];
            int start=0;int last=i,pos,mid;
            while(last-start>1)
            {

                mid=(start+last)/2;
                if(p[mid].second<=p[i].first)
                    {
                        start=mid;
                    }
                else if(p[mid].second>p[i].first)
                    last=mid;
            }
            if(p[start].second>p[i].first)
                    dp[i]=(dp[i]%MOD+1%MOD)%MOD;
                else
                    dp[i]=(dp[i]%MOD+dp[start]%MOD+1%MOD)%MOD;
            /*for(int i=0;i<n;i++)
            cout<<dp[i]<<" ";
            cout<<endl;*/


        }

        cout<<setw(8)<<setfill('0')<<dp[n-1]<<endl;
        //cout<<dp[n-1]<<endl;
        cin>>n;


    }
    return 0;
}
