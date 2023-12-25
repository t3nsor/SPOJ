// 2009-03-02
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
int buf[100000];
void mergesort(int* a,int l,int r)
{
	if (r==l) return;
	int m=(l+r)/2;
	mergesort(a,l,m);
	mergesort(a,m+1,r);
	int i=0,j=l,k=m+1;
	while (j<=m&&k<=r)
		if (a[j]<=a[k])
			buf[i++]=a[j++];
		else
			buf[i++]=a[k++];
	while (j<=m) buf[i++]=a[j++];
	while (k<=r) buf[i++]=a[k++];
	memcpy(a+l,buf,sizeof(int)*(r-l+1));
}
int process(char* s,int* p)
{
	int* init=p;
	char c;
	int x;
	for(;;)
	{
		x=0;
		for(;;)
		{
			if ((c=*s++)<=32) break;
			x=(x<<1)+(x<<3)+c-'0';
		}
		*p++=x;
		if (!c) return p-init;
	}
}
int main()
{
	int N,i,a[100000];
	static char input[1100000];
	gets(input);
	N=process(input,a);
	sort(a,a+N);
	for (i=0; i<N-1; i++)
		printf("%d ",a[i]);
	printf("%d\n",a[N-1]);
	return 0;
}
