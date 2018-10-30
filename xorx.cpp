#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define o 100005
#define MOD 1000000007
#define pb push_back
#define mp make_pair
#define mem(a,b) memset(a,(b),sizeof(a))
#define lpo(a) for(ll i=0;i<a;i++)
#define loop(a,x) for(ll i=a;i<x;i++)
#define srt(x) sort(x.begin(),x.end())
#define all(x) x.begin(),x.end()
const ll MAX=1000000000000000001;
typedef pair<ll,ll> pii;
const ll INT_SIZE = 32 ;
ll m;
struct trie 
{ 
    ll value;  
    trie *arr[2]; 
}; 
trie *newNode() 
{ 
    trie *temp = new trie; 
    temp->value = 0; 
    temp->arr[0] = temp->arr[1] = NULL; 
    return temp; 
} 
void insert(trie *root, ll pre_xor) 
{ 
    trie *temp = root; 
    for (ll i=INT_SIZE-1; i>=0; i--) 
    { 
        bool val = pre_xor & (1<<i); 
        if (temp->arr[val] == NULL) 
            temp->arr[val] = newNode(); 
  
        temp = temp->arr[val]; 
    } 
    temp->value = pre_xor; 
} 
ll query(trie *root, ll pre_xor) 
{ 
    trie *temp = root; 
    for (ll i=INT_SIZE-1; i>=0; i--) 
    { 
        bool val = pre_xor & (1<<i); 
        if (temp->arr[1-val]!=NULL) 
            temp = temp->arr[1-val]; 
        else if (temp->arr[val] != NULL) 
            temp = temp->arr[val]; 
    } 
    return pre_xor^(temp->value); 
} 
ll fun(ll arr[], ll n) 
{ 
    trie *root = newNode(); 
    insert(root, 0); 
    ll result = INT_MIN, pre_xor =0; 
    ll ans=0;
    ll id;
    for (ll i=0; i<n; i++) 
    { 
        pre_xor = pre_xor^arr[i]; 
        ll p=query(root, pre_xor^m); 
        if(ans<p)
        {
        	ans=p;
        	id=i;
        }
        insert(root, pre_xor); 
    } 
    ll xo=0;
    for(ll i=id;i>=0;i--)
    {
    	xo=(xo^arr[i]);
    	if((xo^m)==ans)
    	{
    		return xo;
    	}
    }
} 
int main() 
{ 
	ll t;
	cin>>t;
	while(t--)
	{
		ll n;
		cin>>n>>m;
		ll a[n];
		lpo(n)
		{
			cin>>a[i];
		}
		cout<<fun(a,n)<<endl;
	}
    return 0; 
} 