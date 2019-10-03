DGE#include<bits/stdc++.h>
using namespace std;
#define fast ios_base::sync_with_stdio(false);	cin.tie(NULL);	cout.tie(NULL);
#define ll long long
#define pb push_back
#define pii pair<int,int>
#define INF 0x3f3f3f3f
#define mod 1000000007

struct comp
{
       bool operator()(pii &a, pii &b)
       {
           if(a.first!=b.first)
                  return a.first<b.first;
           else
                  return a.second<b.second;
       }
};

int main()
{
	fast;
	int t,n;
	cin>>t;
	while(t--)
	{
        vector<pii> v;
        vector<int> dp(1005,1), a(1005), b(1005);
		    cin>>n;
        int x;
		    for(int i=0;i<n;i++)
            cin>>a[i];
		    for(int i=0;i<n;i++)
            cin>>b[i];
        for(int i=0;i<n;i++)
            v.pb(pii(a[i],b[i]));
        sort(v.begin(),v.end(),comp());
        for(int i=0;i<n;i++)
        {
          for(int j=0;j<i;j++)
          {
            if(v[i].first>=v[j].first && v[i].second>=v[j].second && dp[i]<1+dp[j])
              dp[i]=dp[j]+1;
          }
        }
        int maxx=-1;
            for(auto it:dp)
                maxx=max(maxx,it);
        cout<<maxx<<"\n";
	}	
	return 0;
}
