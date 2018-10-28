#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int mod =1000000007;
ll modexpo(ll x,ll y){
    ll ans=1;
    x=(x%mod);
    while(y>0){
        if(y&1)
            ans=((ans%mod)*(x%mod))%mod;
        x=((x%mod)*(x%mod))%mod;
        y>>=1;
    }
    return ans%mod;
}


int main(){
    ll t,n;
    cin>>t;
    while(t--){
        cin>>n;
        ll ans1=(modexpo(10,n)%mod-1+mod)%mod;
        ll ans2=((50%mod)*(ans1%mod))%mod;
        ll ans3=((36%mod)*(n%mod))%mod;
        ll ans4=((ans2%mod)+(ans3%mod))%mod;
        ll ans5=modexpo(81,1000000005)%mod;

        ll ans=((ans4%mod)*(ans5%mod))%mod;
        //cout<<ans1<<" "<<ans2<<" "<<ans3<<" "<<ans4<<" "<<ans5<<endl;

        cout<<ans%mod<<endl;
    }
return 0;
}
