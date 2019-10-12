#include <bits/stdc++.h>
using namespace std;

int main() {
    long long cs = 1;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	// your code goes here
	while(1){
	    long long n;
	    cin>>n;
	    if(n==0)
	        return 0;
	    long long in,c,f,arr[n];
	    cin>>in>>c>>f;
	    for(int i = 0;i<n;i++){
	        cin>>arr[i];
	    }
	    long long ans = 0;
	    long long cnt = 0;
	    long long k = (in+f)/c;
	    k++;
	    k = min(n,k);
	    //cout<<k<<endl;
	    queue<long long> q;
	    deque<long long> dq;
	    for(int i = 0;i<k;i++){
	        q.push(arr[i]);
	        while(!dq.empty() && dq.back()<arr[i])
	            dq.pop_back();
	        dq.push_back(arr[i]);
	    }
	    for(int i = 0;i<n;i++){
	        //cout<<cnt<<" : ";
	        if(cnt<arr[i]){
	            ans+=(arr[i]-cnt)*in;
	            cnt = arr[i];
	        }
	        //c//out<<i<<":";
	        //cout<<cnt<<endl;
	        ans+=cnt*c;
	        //cout<<ans<<endl;
	        
	        if(q.front() == dq.front())
	            dq.pop_front();
	        q.pop();
	        if(k<n){
	            //cout<<k<<endl;
	            q.push(arr[k]);
	            while(!dq.empty() && dq.back()<arr[k])
	                dq.pop_back();
	            dq.push_back(arr[k]);
	            k++;
	        }
	        
	        if(i!=n-1 && dq.front()<cnt){
	            ans += (cnt-dq.front())*f;
	            cnt = dq.front();
	        }
	        //cout<<ans<<endl;
	    }
	    cout<<"Case "<<cs++<<", cost = $";
	    cout<<ans<<endl;
	}
	return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long cs = 1;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
	// your code goes here
	while(1){
	    long long n;
	    cin>>n;
	    if(n==0)
	        return 0;
	    long long in,c,f,arr[n];
	    cin>>in>>c>>f;
	    for(int i = 0;i<n;i++){
	        cin>>arr[i];
	    }
	    long long ans = 0;
	    long long cnt = 0;
	    long long k = (in+f)/c;
	    k++;
	    k = min(n,k);
	    //cout<<k<<endl;
	    queue<long long> q;
	    deque<long long> dq;
	    for(int i = 0;i<k;i++){
	        q.push(arr[i]);
	        while(!dq.empty() && dq.back()<arr[i])
	            dq.pop_back();
	        dq.push_back(arr[i]);
	    }
	    for(int i = 0;i<n;i++){
	        //cout<<cnt<<" : ";
	        if(cnt<arr[i]){
	            ans+=(arr[i]-cnt)*in;
	            cnt = arr[i];
	        }
	        //c//out<<i<<":";
	        //cout<<cnt<<endl;
	        ans+=cnt*c;
	        //cout<<ans<<endl;
	        
	        if(q.front() == dq.front())
	            dq.pop_front();
	        q.pop();
	        if(k<n){
	            //cout<<k<<endl;
	            q.push(arr[k]);
	            while(!dq.empty() && dq.back()<arr[k])
	                dq.pop_back();
	            dq.push_back(arr[k]);
	            k++;
	        }
	        
	        if(i!=n-1 && dq.front()<cnt){
	            ans += (cnt-dq.front())*f;
	            cnt = dq.front();
	        }
	        //cout<<ans<<endl;
	    }
	    cout<<"Case "<<cs++<<", cost = $";
	    cout<<ans<<endl;
	}
	return 0;
}
