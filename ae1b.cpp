// 2009-12-21
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
int main()
{
	int N,f1,f2,K,i,res=0;
	int a[1000];
	scanf("%d %d %d",&N,&f1,&f2);
	K=f1*f2;
	for (i=0; i<N; i++)
		scanf("%d",a+i);
	sort(a,a+N,greater<int>());
	for (i=0; res<K; i++)
		res+=a[i];
	printf("%d\n",i);
	return 0;
}
