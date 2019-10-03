#include<bits/stdc++.h>
using namespace std;
#define fast ios_base::sync_with_stdio(false);	cin.tie(NULL);	cout.tie(NULL);
#define ll long long
#define pb push_back
#define fi first
#define se second
#define pii pair<ll,ll>
#define INF 0x3f3f3f3f
#define mod 1000000007
#define testcases() int t; cin>>t; while(t--)
#define debug1(x) cout<<#x<<" "<<x<<"\n";
#define debug2(x,y) cout<<#x<<" "<<x<<" "<<#y<<" "<<y<<"\n";
#define debug3(x,y,z) cout<<#x<<" "<<x<<" "<<#y<<" "<<y<<" "<<#z<<" "<<z<<"\n";
 //----------------------------------------------------------------------------------------------------------------------------------------------------

struct info
{
	int ans;
	int left;
	int right;
};

vector<info> st;
string str;

void build_st(int node, int beg, int end)
{
	if(beg==end)
	{
		if(str[beg]=='(')
			st[node]={0,1,0};
		else
			st[node]={0,0,1};
		return;
	}
	int mid=beg+(end-beg)/2;
	build_st(2*node+1,beg,mid);
	build_st(2*node+2,mid+1,end);
	int x=min(st[2*node+1].left,st[2*node+2].right);
	st[node].ans=st[2*node+1].ans+st[2*node+2].ans+2*x;
	st[node].left=st[2*node+1].left+st[2*node+2].left-x;
	st[node].right=st[2*node+1].right+st[2*node+2].right-x;
}

void update(int node, int beg, int end, int pos)
{
	int mid=beg+(end-beg)/2;
	if(beg==end)
	{
		st[node].left^=1;
		st[node].right^=1;
		return;
	}
	if(pos<=mid)
		update(2*node+1,beg,mid,pos);
	else
		update(2*node+2,mid+1,end,pos);
	int x=min(st[2*node+1].left,st[2*node+2].right);
	st[node].ans=st[2*node+1].ans+st[2*node+2].ans+2*x;
	st[node].left=st[2*node+1].left+st[2*node+2].left-x;
	st[node].right=st[2*node+1].right+st[2*node+2].right-x;
}

int main()
{
	fast;
	int n,q,x,k=0;
	while(cin>>n)
	{
		cin.ignore();
		cin>>str;
		st.resize(4*n+5);
		build_st(0,0,n-1);
		cout<<"Test "<<++k<<":\n";
		cin>>q;
		for(int i=0;i<q;i++)
		{
			cin>>x;
			--x;
			if(x==-1)
      {
           //debug1(st[0].ans);
           st[0].ans==n?cout<<"YES\n":cout<<"NO\n";
      }
			else
    			 update(0,0,n-1,x);
		}
	}
  return 0;
}
